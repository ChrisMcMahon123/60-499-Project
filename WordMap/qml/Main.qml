import VPlayApps 1.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.3

//library name comes from main.cpp
import BackEnd 1.2
import ToDoModel 1.0

App {
    id: root
    height: 700
    width: 499 //changes to 500 to 'fix' layout issue on window load

    //work around for the layouts not properly being set on window load
    //oddly enough this works 95% of the time. need to look into this more ***
    Component.onCompleted: {
        onLoad()
    }

    function onLoad() {
        root.width = root.width +1
    }

    Item {
        id: localVariables
        property string settingsType: ""
        property string fileFilter: ""
        property int scriptStatus: 0
    }

    BackEnd {
        id: backEnd
    }

    // You get free licenseKeys from https://v-play.net/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the V-Play Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://v-play.net/licenseKey>"

    //scrollable container that holds all of the UI elements
    ScrollView {
        id: scrollView
        clip: true
        anchors.fill: parent
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 67
        anchors.topMargin: 5

        ScrollBar.horizontal.interactive: false
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: true

        //main container of UI elements
        ColumnLayout {
            anchors.fill: parent
            spacing: 5

            //select text file
            RowLayout {
                width: root.width

                AppText {
                    Layout.alignment: Qt.AlignLeft
                    text: "Source Text"
                    font.pointSize: 11
                }

                AppButton {
                    Layout.alignment: Qt.AlignRight
                    text: qsTr("Select File")
                    onClicked: {
                        localVariables.fileFilter = "Text Files (*.txt)"
                        localVariables.settingsType = "Text"
                        fileDialog.visible = true
                    }
                }
            }

            //scrollable textarea
            Rectangle {
                id: textAreaBackground
                height: 400
                width: root.width
                color: "lightgrey"

                Flickable {
                    anchors.fill: parent

                    TextArea.flickable: textArea

                    //adding a scrollbar to the flickable area
                    ScrollBar.vertical: ScrollBar {
                        active: true
                        onActiveChanged: {
                            //show the scrollbar when needed
                            if (!active) {
                                active = true;
                            }
                        }
                    }
                }

                TextArea {
                    id: textArea
                    font.pointSize: 11
                    wrapMode: "Wrap"
                    selectByMouse: true
                }
            }

            //text filter list
            RowLayout {
                width: root.width

                AppText {
                    text: "Text Filter List"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                AppButton {
                    text: qsTr("Edit List")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        ignoreListDialog.visible = true
                    }
                }
            }

            //divider
            Rectangle {
                height: 1
                width: root.width
                color: "black"
            }

            AppText {
                text: "Formatting and Styling"
                font.pointSize: 11
                Layout.topMargin: 5
            }

            //font style
            RowLayout {
                width: root.width

                AppText {
                    text: "Font Style"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                AppButton {
                    text: qsTr("Edit Font")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        localVariables.settingsType = "Font"
                        fontDialog.visible = true
                    }
                }
            }

            //Color picker for the font
            RowLayout {
                width: root.width

                AppText {
                    text: "Font Color"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                AppButton {
                    text: qsTr("Edit Color")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        localVariables.settingsType = "Font"
                        colorDialog.visible = true
                    }
                }
            }

            //background Color
            RowLayout {
                width: root.width

                AppText {
                    text: "Background Color"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                AppButton {
                    text: qsTr("Edit Color")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        localVariables.settingsType = "Background"
                        colorDialog.visible = true
                    }
                }
            }

            //Color picker for the font
            RowLayout {
                width: root.width

                AppText {
                    text: "Background Image"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                AppButton {
                    text: qsTr("Select Image")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        localVariables.fileFilter = "Image Files (*.jpg *.png)"
                        localVariables.settingsType = "Image"
                        fileDialog.visible = true
                    }
                }
            }

            //Color picker for the font
            RowLayout {
                width: root.width

                AppText {
                    text: "Background Shape"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                ComboBox {
                    id: backgroundShapeComboBox
                    Layout.alignment: Qt.AlignRight
                    model: ["Square", "Rectangle", "Circle", "Triangle"]
                    onCurrentIndexChanged: {
                        backEnd.backgroundShape(backgroundShapeComboBox.currentIndex)
                    }
                }
            }
        }
    }

    //bottom controls area
    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 7
        anchors.leftMargin: 7

        //divider
        Rectangle {
            height: 1
            width: root.width
            color: "black"
        }

        //reset and generate word map buttons
        RowLayout {
            Layout.alignment: Qt.AlignVCenter
            width: root.width

            AppButton {
                text: qsTr("Reset Settings")
                Layout.alignment: Qt.AlignLeft
                onClicked: {
                    backEnd.resetInputs()
                    textArea.text = ""
                }
            }

            AppButton {
                text: qsTr("Generate Wordmap")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    if(backEnd.generateWordMap(textArea.text) === 0) {
                        //script has an error, notify the user
                        wordMapErrorDialog.visible = true
                    }
                    else {
                        wordMapPreviewDialog.visible = true
                    }
                }
            }
        }
    }

    //file dialog for selecting text files
    FileDialog {
        id: fileDialog
        title: qsTr("Select " + localVariables.settingsType + " File")
        folder: shortcuts.home
        nameFilters: localVariables.fileFilter
        visible: false
        onAccepted: {
            //open file stream and get the contents of the file
            //put the contents of the file into the textarea
            //console.log("Selected File: " + fileDialog.fileUrls)
            if(localVariables.settingsType == "Text") {
                textArea.text = backEnd.textFileContents(fileDialog.fileUrl);
            }
            else if(localVariables.settingsType == "Image") {
                backEnd.backgroundImageUrl = fileDialog.fileUrl
            }
        }
    }

    FontDialog {
        id: fontDialog
        title: qsTr("Edit " + localVariables.settingsType + " Style")
        visible: false
        onAccepted: {
            //update font settings
            //console.log("Selected Font: " + fontDialog.currentFont)
            backEnd.fontStyle = fontDialog.currentFont;
        }
    }

    ColorDialog {
        id: colorDialog
        title: qsTr("Edit " + localVariables.settingsType + " Color")
        visible: false
        onAccepted: {
            //update Color settings
            //console.log("Selected Color: " + ColorDialog.currentColor)
            if(localVariables.settingsType == "Font") {
                backEnd.fontColor = colorDialog.currentColor;
            }
            else if(localVariables.settingsType == "Background") {
                backEnd.backgroundColor = colorDialog.currentColor;
            }
        }
    }

    //ignore list dialog to add and remove entries
    Dialog {
        width: 450
        visible: false
        id: ignoreListDialog
        title: qsTr("Edit Ignore List Dialog")

        contentItem: ColumnLayout {
            anchors.fill: parent

            Rectangle {
                height: 400
                width: ignoreListDialog.width

                ListView {
                    anchors.fill: parent
                    clip: true

                    model: ToDoModel {
                        list: toDoList
                    }

                    delegate: RowLayout {
                        width: parent.width

                        CheckBox {
                            checked: model.done
                            onClicked: model.done = checked
                        }

                        Rectangle {
                            color:"lightgrey"
                            width: 250
                            height: 45
                            clip: true

                            AppTextEdit {
                                Layout.leftMargin: 15
                                height: parent.height
                                width: parent.width
                                text: model.description
                                onEditingFinished: model.description = text
                                selectByMouse: true
                            }
                        }

                        AppButton {
                            Layout.alignment: Qt.AlignRight
                            backgroundColor: "Red"
                            text: qsTr("Remove")
                            onClicked: {
                                console.log("Remove Word: " + model.description)
                            }
                        }
                    }
                }
            }

            //divider
            Rectangle {
                height: 1
                width: ignoreListDialog.width
                color: "black"
            }

            RowLayout {
                width: ignoreListDialog.width

                Rectangle {
                    color: "lightgrey"
                    width: 290
                    height: 45
                    clip: true
                    Layout.leftMargin: 10
                    Layout.alignment: Qt.AlignLeft

                    AppTextEdit {
                        id: ignoreWord
                        height: parent.height
                        width: parent.width
                        placeholderText: qsTr("Ignore Word")
                        selectByMouse: true
                    }
                }

                AppButton {
                    Layout.alignment: Qt.AlignRight
                    text: qsTr("Add Word")
                    onClicked: {
                        toDoList.appendItem()
                        console.log("Add Word to Ignore List")
                    }
                }
            }

            DialogButtonBox {
                Layout.alignment: Qt.AlignRight

                standardButtons: Dialog.Close

                onRejected: {
                    ignoreListDialog.visible = false
                }
            }
        }
    }

    //word map error dialog
    Dialog {
        visible: false
        id: wordMapErrorDialog
        title: qsTr("Error Generating Word Map")

        contentItem: ColumnLayout {
            RowLayout {
                Image {
                    source: "../assets/icons8-cancel-50.svg"
                }
                AppText {
                    text: "Source text has not been filled out"
                }
            }

            DialogButtonBox {
                Layout.alignment: Qt.AlignRight

                standardButtons: Dialog.Close

                onRejected: {
                    wordMapErrorDialog.visible = false
                }
            }
        }
    }

    //word map dialog
    Dialog {
        height: 600
        width: 600
        visible: false
        id: wordMapPreviewDialog
        title: qsTr("Word Map Preview")
    }
}
