#include "cursorcommand.h"
#include "QTextBlock"

CursorCommand::CursorCommand(QTextDocument* doc){
    this->doc=doc;
}

void CursorCommand::Indent(int cursorPosition,int width){
    int position=cursorPosition;
    int existingIndent=0;
    QString code=doc->toPlainText();
    while(position>0&&code[position-1]!='\n'){
        position--;
        existingIndent++;
    }
    int num=width-existingIndent%width;
    QTextCursor cursor(doc);
    cursor.setPosition(cursorPosition);
    for(int i=0;i<num;i++)
        cursor.insertText(" ");
}

void CursorCommand::unIndent(int cursorPosition, int width) {
    int position = cursorPosition;
    int existingIndent = 0;
    QString code = doc->toPlainText();
    while (position > 0 && code[position - 1] != '\n') {
        position--;
        existingIndent++;
    }
    int num = existingIndent % width;
    if(num==0)
        num=4;
    QTextCursor cursor(doc);
    cursor.setPosition(cursorPosition);
    for (int i = 0; i < num; i++) {
        if (cursor.atStart() || cursor.atBlockStart())
            break;
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        if (cursor.selectedText() == " ")
            cursor.deleteChar();
        else
            break;
    }
}

void CursorCommand::CommentLine(int position) {
    QTextCursor cursor(doc);
    cursor.setPosition(position);
    cursor.movePosition(QTextCursor::StartOfLine);
    QString lineText = cursor.block().text();
    if (lineText.startsWith("//")) {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
        cursor.removeSelectedText();
    }else{
        cursor.insertText("//");
    }
}

void CursorCommand::ToggleCommentBlock(int beginPosition, int endPosition) {
    QTextCursor cursor(doc);

    // Set the cursor positions to the specified begin and end positions
    QTextBlock beginBlock = doc->findBlock(beginPosition);
    QTextBlock endBlock = doc->findBlock(endPosition);
    cursor.beginEditBlock(); // Start a block of editing operations

    // Check if any line within the selected block does not start with "//"
    bool addComment = false;
    cursor.setPosition(beginBlock.position()); // Set the cursor to the beginning of the block
    while (true){
        QString lineText = cursor.block().text().trimmed();
        if (!lineText.startsWith("//")) {
            addComment = true;
            break;
        }
        if(cursor.block().position() == endBlock.position())
            break;
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.setPosition(beginBlock.position()); // Reset the cursor to the beginning of the block

    // Apply comment/uncomment to the entire block
    while (true) {
        QString blockText = cursor.block().text();
        if (addComment) {
            // Add "//" comment to each line in the block
            cursor.insertText("//");
        } else {
            // Remove "//" comment from each line in the block
            cursor.setPosition(cursor.block().position()); // Set cursor to start of the line
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2); // Select "//"
            cursor.removeSelectedText(); // Remove the selected "//"
        }
        if(cursor.block().position() == endBlock.position())
            break;
        cursor.movePosition(QTextCursor::NextBlock); // Move to the next line
    }

    cursor.endEditBlock(); // End the block of editing operations
}

