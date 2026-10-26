#include "ui/common/Components.h"
#include "ui/common/Theme.h"
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>

namespace p1::ui {
QLabel* label(const QString& text, const char* role, QWidget* parent) {
    auto* result = new QLabel(text, parent);
    result->setProperty("role", role);
    result->setWordWrap(true);
    result->setTextFormat(Qt::PlainText);
    result->setMinimumWidth(0);
    return result;
}
QLabel* badge(const QString& text, const char* tone, QWidget* parent) {
    auto* result = label(text, "caption", parent);
    result->setProperty("badge", tone);
    result->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    return result;
}
QPushButton* button(const QString& text, bool primary, QWidget* parent) {
    auto* result = new QPushButton(text, parent);
    result->setMinimumHeight(44);
    if (primary) result->setProperty("variant", "primary");
    result->setCursor(Qt::PointingHandCursor);
    return result;
}
QWidget* scrollable(QWidget* content) {
    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidget(content);
    return scroll;
}
Card::Card(Theme& theme, QWidget* parent) : QFrame(parent), body_(new QVBoxLayout(this)) {
    setProperty("card", true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    body_->setContentsMargins(24,24,24,24);
    body_->setSpacing(16);
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(14); shadow->setOffset(0,4);
    auto update = [shadow, &theme] {
        shadow->setColor(theme.isDark() ? QColor(0,0,0,51) : QColor(68,48,105,26));
    };
    update();
    QObject::connect(&theme, &Theme::changed, this, update);
    setGraphicsEffect(shadow);
}
}
