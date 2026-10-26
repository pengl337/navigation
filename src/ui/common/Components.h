#pragma once

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace p1::ui {
class Theme;
QLabel* label(const QString& text, const char* role = "body", QWidget* parent = nullptr);
QLabel* badge(const QString& text, const char* tone = "info", QWidget* parent = nullptr);
QPushButton* button(const QString& text, bool primary = false, QWidget* parent = nullptr);
QWidget* scrollable(QWidget* content);

class Card final : public QFrame {
public:
    explicit Card(Theme& theme, QWidget* parent = nullptr);
    QVBoxLayout* body() const { return body_; }
private:
    QVBoxLayout* body_;
};
}
