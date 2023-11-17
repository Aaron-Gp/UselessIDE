#ifndef CURSORCOMMAND_H
#define CURSORCOMMAND_H

#include <QTextCursor>
#include <QTextDocument>



class CursorCommand
{
public:
    CursorCommand(QTextDocument* doc);
    QTextDocument* doc;
    void Indent(int cursorPosition,int width);
    void unIndent(int cursorPosition, int width);
    void CommentLine(int position);
    void ToggleCommentBlock(int beginPosition, int endPosition);
};

#endif // CURSORCOMMAND_H
