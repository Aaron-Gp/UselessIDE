#include "store.h"

Store::Store(QObject *parent)
    : QObject{parent}
{
//    m_highlighter = new SyntaxHighlighter();

}

Store* Store::instance() {
    static Store *store = new Store();
    return store;
}

