import VPlayApps 1.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.3

//library name comes from main.cpp
import WordMap.BackEnd 1.0

App {
    id: root
    height: 700
    width: 500

    Item {
        id: localVariables
        property string settingsType: ""
        property string fileFilter: ""

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
                //issue, alignment not set until screen resize
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

                    TextArea.flickable: textEdit

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
                    id: textEdit
                    font.pointSize: 11
                    wrapMode: "Wrap"
                }
            }

            //text filter list
            RowLayout {
                //issue, alignment not set until screen resize
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
                        textFilterDialog.visible = true
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
                //issue, alignment not set until screen resize
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
                //issue, alignment not set until screen resize
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
                //issue, alignment not set until screen resize
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
                //issue, alignment not set until screen resize
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
                //issue, alignment not set until screen resize
                width: root.width

                AppText {
                    text: "Background Shape"
                    font.pointSize: 11
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                ComboBox {
                    id: backgroundShapeComboBox
                    Layout.alignment: Qt.AlignRight
                    width: 250
                    model: ["Square", "Rectangle", "Circle"]
                    onStateChanged: {
                        console.log(backgroundShapeComboBox)
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
            //issue, alignment not set until screen resize
            width: root.width

            AppButton {
                text: qsTr("Reset Settings")
                Layout.alignment: Qt.AlignLeft
                onClicked: {
                    console.log("Reset Settings")
                }
            }

            AppButton {
                text: qsTr("Generate Wordmap")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    console.log("Generate Wordmap")
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
            //console.log("Selected Color: " + ColorDialog.currentColor + " | Selected Alpha: " + ColorDialog.currentAlpha)
            if(localVariables.settingsType == "Font") {
                backEnd.fontColor = colorDialog.currentColor;
                backEnd.fontAlpha = colorDialog.currentAlpha;
            }
            else if(localVariables.settingsType == "Background") {
                backEnd.backgroundColor = colorDialog.currentColor;
                backEnd.backgroundAlpha = colorDialog.currentAlpha;
            }
        }
    }

    //text filter dialog to add and remove entries
    Dialog {
        height: 500
        width: 400
        visible: false
        id: textFilterDialog
        title: qsTr("Edit Text Filter List")

    }
}
