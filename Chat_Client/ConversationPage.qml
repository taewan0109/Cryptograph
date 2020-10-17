import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import io.qt.examples.chattutorial 1.0

Page {
    id: root

    property string inConversationWith

    header: ChatToolBar {   //채팅 툴바 부분
        ToolButton {        //Back 버튼
            text: qsTr("Back")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.pop()
        }

        Label {     //상대방 이름
            id: pageTitle
            text: inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }


    ColumnLayout {  //메세지 관련 부분
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
            verticalLayoutDirection: ListView.BottomToTop
            spacing: 12
            model: SqlConversationModel {
                recipient:inConversationWith
            }

            delegate: Column {
                anchors.right: sentByMe ? parent.right : undefined

                spacing: 6

                readonly property bool sentByMe: model.recipient !== "Me"

                Row {
                    id: messageRow
                    spacing: 6
                    anchors.right: sentByMe ? parent.right : undefined

                    Image { //상대방 이미지 표시
                        id: avatar
                        source: !sentByMe ? "qrc:/" + model.author.replace(" ", "_") + ".png" : ""  //내 메세지가 아니라면 사진 표시
                    }

                    Rectangle { //메세지 박스
                        width: Math.min(messageText.implicitWidth + 24, listView.width - avatar.width - messageRow.spacing)
                        height: messageText.implicitHeight + 24
                        //color: sentByMe ? "lightgrey" : "steelblue"
                        color: sentByMe ? "#41cd52" : "lightgrey"

                        Label { //메세지 내용, 색
                            id: messageText
                            text: model.message
                            //color: sentByMe ? "black" : "white"
                            color: "black"
                            anchors.fill: parent
                            anchors.margins: 12
                            wrapMode: Label.Wrap
                        }
                    }
                }

                Label { //메세지 시간 표시
                    id: timestampText
                    text: Qt.formatDateTime(model.timestamp, "d/MMM/hh:mm")
                    color: "lightgrey"
                    anchors.right: sentByMe ? parent.right : undefined
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Pane {
            id: pane
            Layout.fillWidth: true

            RowLayout {
                width: parent.width

                TextArea {  //메세지 작성하는 필드
                    id: messageField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Compose message")
                    wrapMode: TextArea.Wrap
                }

                Button {    //전송 버튼
                    id: sendButton
                    text: qsTr("Send")
                    enabled: messageField.length > 0    //메세지가 작성되있을때만 활성화
                    onClicked: {    //클릭되면
                        listView.model.sendMessage(inConversationWith, messageField.text);  //메세지 전송
                        messageField.text = ""; //메세지 필드 초기화
                    }
                    Shortcut {
                        sequence: "ctrl+s"
                        onActivated: sendButton.clicked()
                    }
                }
            }
        }
    }
}

