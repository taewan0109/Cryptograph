import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: window
    width: 400//540
    height: 620//960
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: ContactPage {}
    }
}
