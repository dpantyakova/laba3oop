#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

// Базовый класс Editor
class Editor {
public:
    Editor() {
        text = "";
    }

    Editor(const string& initialText) {
        text = initialText;
    }

    void Insert(const string& newText, size_t position) {
        text.insert(position, newText);
    }

    void Delete(size_t position, size_t length) {
        text.erase(position, length);
    }

    string GetText() const {
        return text;
    }

    // Виртуальный метод идентификации в базовом классе
    virtual string identify() const {
        return "Editor";
    }

protected:
    string text;
};

// Объявление структуры Operation и stack здесь
struct Operation {
    char type;
    string text;
    size_t position;

    Operation(char t, const string& txt, size_t pos) : type(t), text(txt), position(pos) {}
};

// Производный класс TextEditor
class TextEditor : public Editor {
public:
    TextEditor() : Editor() {}

    TextEditor(const string& initialText) : Editor(initialText) {}

    // Переопределение метода идентификации для TextEditor
    string identify() const override {
        return "TextEditor";
    }

    // Добавляем функциональность для истории операций в TextEditor
    bool Undo() {
        if (!history.empty()) {
            Operation lastOperation = history.top();
            history.pop();

            if (lastOperation.type == 'I') {
                Editor::text.erase(lastOperation.position, lastOperation.text.length());
            } else if (lastOperation.type == 'D') {
                Editor::text.insert(lastOperation.position, lastOperation.text);
            } else if (lastOperation.type == 'R') {
                Editor::text.replace(lastOperation.position, lastOperation.text.length(), lastOperation.text);
            }

            return true;
        }
        return false;
    }

    void Insert(const string& newText, size_t position) {
        Editor::Insert(newText, position);
        history.push(Operation('I', newText, position));
    }

    void Delete(size_t position, size_t length) {
        Editor::Delete(position, length);
        string deletedText = Editor::text.substr(position, length);
        history.push(Operation('D', deletedText, position));
    }

    void Replace(const string& newText, size_t position, size_t length) {
        if (position <= Editor::text.length() && length > 0) {
            size_t endIndex = position + length;
            if (endIndex > Editor::text.length()) {
                endIndex = Editor::text.length();
            }

            string replacedText = Editor::text.substr(position, endIndex - position);
            Editor::text.replace(position, endIndex - position, newText);
            history.push(Operation('R', replacedText, position));
        }
    }

private:
    stack<Operation> history;
};

// Тестирующая функция
void runTests() {
    const size_t REPLACE_POSITION = 7;
    const size_t REPLACE_LENGTH = 5;
    const size_t INSERT_POSITION = 4;

    TextEditor editor;
    editor.Insert("Hello, world", 0);
    assert(editor.GetText() == "Hello, world" && "Test failed: Insert initial text");

    editor.Replace("Sir!", REPLACE_POSITION, REPLACE_LENGTH);
    assert(editor.GetText() == "Hello, Sir!" && "Test failed: Replace text");

    editor.Delete(0, REPLACE_POSITION);
    editor.Insert("?", INSERT_POSITION);
    assert(editor.GetText() == "Sir!?" && "Test failed: Delete and Insert text");

    assert(editor.Undo() && editor.GetText() == "Sir!" && "Test failed: Undo operation");

    // Тест метода идентификации
    assert(editor.identify() == "TextEditor" && "Test failed: Incorrect identification for TextEditor");

    cout << "All tests passed!" << endl;
}

int main() {
    runTests();

    return 0;
}
