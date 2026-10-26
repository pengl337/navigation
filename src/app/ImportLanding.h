#pragma once
#include <QWidget>

class QLabel;
class QPushButton;
namespace p1::ui { class Theme; class Card; }
namespace p1::app {
// Application-owned landing shell. It selects a file but performs no parsing/storage.
// Member 2 can replace this entire page through MainWindow::registerPage().
class ImportLanding final : public QWidget {
    Q_OBJECT
public:
    explicit ImportLanding(ui::Theme& theme, QWidget* parent = nullptr);
    QString selectedFile() const { return selectedFile_; }
    void selectFile(const QString& path);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QString selectedFile_;
    QLabel* fileName_;
    QLabel* fileInfo_;
    ui::Card* picker_;
};
}
