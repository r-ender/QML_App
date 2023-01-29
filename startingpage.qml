import QtQuick 2.6
import QtQuick.Controls 2.0

ApplicationWindow{

    //id: _start
    visible: true
    width: 600
    height: 680
    color: "steelblue"

    StackView{
        id: _stack
        anchors.fill: parent
        initialItem: Frontpage { }
    }
}
    /*
    Page {
        anchors.fill: parent
        header: Label{
            padding: 10
            text: "This is MoTalk-App"
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

    Image {
            id: _backgroundFreq
            anchors.fill: parent
            width: parent.width
            height: parent.height
            source: "background_frequency.jpg"
            fillMode: Image.Tile; opacity: 0.3
    }


   StackView{
       id: _stack
       anchors.fill: parent
       initialItem: _frontpage
   }



   Button{
       anchors.centerIn: parent
       text: "Click me!"
       onClicked: {
           var component = Qt.createComponent("main.qml");
           var window = component.createObject(_start);
           window.show();
       }

   }


   Component{
       id: _frontpage

               ListView{
                       id: _listView
                       anchors.fill: parent
                       topMargin: 48
                       leftMargin: 48
                       bottomMargin: 48
                       rightMargin: 48
                       spacing: 20
                       model: ["Marco Hiller", "Stefan Lex"]
                       delegate: ItemDelegate {        //each ItemDelegate can display text, be checked on/off and react to mouse clicks
                           text: modelData
                           width: _listView.width - _listView.leftMargin - _listView.rightMargin
                           height: _avatar.implicitHeight
                           leftPadding: _avatar.implicitWidth + 32

                           //turn to next page when clicked
                           onClicked: {
                               //_stack.pop(_frontpage, _stack.Immediate);
                               //_stack.pop();
                               _stack.push("qrc:/main.qml", { inConversationWith: model.display});
                               //_stack.clear(_frontpage);
                           }

                           Image {
                               id: _avatar
                               source: "qrc:/" + modelData.replace(" ", "_") + ".jpeg"
                           }
                       }
                   }
       }
   }
}
*/
