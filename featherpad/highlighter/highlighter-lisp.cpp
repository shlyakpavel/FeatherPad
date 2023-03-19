/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2021-2022 <tsujan2000@gmail.com>
 *
 * FeatherPad is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FeatherPad is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <https://spdx.org/licenses/GPL-3.0+.html>
 */

#include "highlighter.h"

#include <QTextBlockUserData>

namespace FeatherPad {


void Highlighter::highlightLispBlock(const QString &text) {
    setCurrentBlockState(0);

    TextBlockData *data = new TextBlockData;
    setCurrentBlockUserData(data);

    // Clear formatting
    setFormat(0, text.length(), mainFormat);

    // Parentheses
    QRegularExpression parenRegex("(\\(|\\))");
    int parenIndex = text.indexOf(parenRegex);
    while (parenIndex >= 0) {
        QRegularExpressionMatch match = parenRegex.match(text, parenIndex);
        char ch = match.captured(0).at(0).toLatin1();
        ParenthesisInfo *pInfo = new ParenthesisInfo;
        pInfo->character = ch;
        pInfo->position = parenIndex;
        data->insertInfo(pInfo);
        setFormat(parenIndex, 1, blockQuoteFormat); // Adjust the format to your preference
        parenIndex = text.indexOf(parenRegex, parenIndex + 1);
    }

    // Lisp comments
    int commentIndex = text.indexOf(';');
    while (commentIndex != -1) {
        setFormat(commentIndex, text.length() - commentIndex, commentFormat);
        commentIndex = text.indexOf(';', commentIndex + 1);
    }

    // Keywords
    QStringList keywords = {
        "defun", "setq", "let", "lambda", "if", "cond", "case", "loop", "progn",
        "car", "cdr", "cons", "list", "append", "reverse", "length", "nth", "elt", "member",
        "null", "not", "and", "or", "t", "nil", "eq", "equal", "eql", "equalp", "string=",
        "plusp", "minusp", "zerop", "oddp", "evenp", "abs", "min", "max", "floor", "ceiling", "truncate",
        "round", "mod", "rem", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "sqrt", "expt", "log",
        "read", "print", "format", "prin1", "princ", "terpri", "fresh-line", "with-output-to-string",
        "with-input-from-string", "make-string-input-stream", "make-string-output-stream", "get-output-stream-string"
    };

    for (const QString &keyword : keywords) {
            QRegularExpression keywordRegex("\\b" + keyword + "\\b");
            int keywordIndex = text.indexOf(keywordRegex);
            while (keywordIndex >= 0) {
                QRegularExpressionMatch match = keywordRegex.match(text, keywordIndex);
                int length = match.capturedLength();
                setFormat(keywordIndex, length, codeBlockFormat);
                keywordIndex = text.indexOf(keywordRegex, keywordIndex + length);
            }
    }

    setCurrentBlockUserData(data);
}



}
