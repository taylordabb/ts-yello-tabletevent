import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 1280
    title: qsTr("Hello World")

    // TODO: Add handling for composing press, release, and move events. Probably
    //       use a Timer to decide when a release actually means a click (i.e.
    //       when the duration between is short enough and the distance travelled
    //       between press and release is within some small distance.
    //       Will also need to use some Canvas type and just let the QTabletEvent
    //       get handled by the Canvas for things like a signature screen.
    Connections {
        target: tabletMouseConverter
        ignoreUnknownSignals: true

        onTabletPress: {
            console.log("Got a tablet press at x,y:", tabletX, tabletY);
        }

        onTabletRelease: {
            console.log("Got a tablet release at x,y:", tabletX, tabletY);
            console.log("firing mouseclick event...");
            tabletMouseConverter.fireMouseClickEvent(tabletX, tabletY);
        }
    }

    Rectangle {
        id: red
        color: "red"
        anchors {
            left: parent.left
            top: parent.top
        }
        width: 200
        height: 200

        MouseArea {
            id: redMA
            anchors.fill: parent

            onClicked: {
                console.log("Clicked the red square");
            }
        }
    }

    Rectangle {
        color: "blue"
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        width: 200
        height: 200

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Clicked the blue square");
            }
        }
    }

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true // Allow un-accepted mouse events to fall through
        onClicked: {
            console.log("Got a click in the window.... x:", mouse.x, "y:", mouse.y);
            mouse.accepted = false; // Set false so MouseArea(s) under this top-level MouseArea receive the event and handle it themselves
        }
    }
}
