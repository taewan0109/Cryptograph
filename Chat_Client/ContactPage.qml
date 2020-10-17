import QtQuick 2.12
import QtQuick.Controls 2.12
import io.qt.examples.chattutorial 1.0

//연락처 목록 qml 코드
Page {
    id: root

    header: ChatToolBar {   //"Contact"툴바 표시
        Label {
            text: qsTr("Contacts")
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    ListView {  //연락처의 리스트 뷰
        id: listView
        anchors.fill: parent
        topMargin: 48
        leftMargin: 48
        bottomMargin: 48
        rightMargin: 48
        spacing: 20
        model: SqlContactModel {}
        delegate: ItemDelegate {
            text: model.display
            width: listView.width - listView.leftMargin - listView.rightMargin
            height: avatar.implicitHeight
            leftPadding: avatar.implicitWidth + 32
            onClicked: root.StackView.view.push("qrc:/ConversationPage.qml", { inConversationWith: model.display })
            //연락처가 클릭되면 다음 페이지로 푸시
            Image {
                id: avatar
                source: "qrc:/" + model.display.replace(" ", "_") + ".png"
            }
        }
    }
}

