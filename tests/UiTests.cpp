#include "app/MainWindow.h"
#include "app/ImportLanding.h"
#include "ui/common/Theme.h"
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPointer>
#include <QProgressBar>
#include <QPushButton>
#include <QScreen>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTemporaryFile>
#include <QtTest>

class UiTests : public QObject {
    Q_OBJECT
private slots:
    void init() {
        theme_ = new p1::ui::Theme(this);
        theme_->apply(*qApp);
        window_ = new p1::app::MainWindow(*theme_);
        window_->show();
        QVERIFY(QTest::qWaitForWindowExposed(window_));
    }
    void cleanup() { delete window_; delete theme_; }

    void initialClientFitsAvailableScreen() {
        const auto available = window_->screen()->availableGeometry().size();
        // On undersized test displays the documented minimum remains authoritative.
        QVERIFY(window_->width() <= qMax(1024,available.width()));
        QVERIFY(window_->height() <= qMax(720,available.height()));
    }

    void themeAndNavigationPreserveForm() {
        QVERIFY(window_->setCurrentPage("components"));
        auto* input = window_->findChild<QLineEdit*>("demoName");
        input->setText("校园数据");
        auto* progress = window_->findChild<QSpinBox*>("demoProgressValue");
        progress->setValue(63);
        QTest::mouseClick(window_->findChild<QPushButton*>("themeButton"),Qt::LeftButton);
        QVERIFY(theme_->isDark());
        QCOMPARE(window_->findChild<QComboBox*>("themeCombo")->currentIndex(),1);
        QCOMPARE(window_->currentPageId(),QString("components"));
        QTest::mouseClick(window_->findChild<QPushButton*>("nav_settings"),Qt::LeftButton);
        QCOMPARE(window_->currentPageId(),QString("settings"));
        window_->findChild<QComboBox*>("themeCombo")->setCurrentIndex(0);
        QVERIFY(!theme_->isDark());
        QVERIFY(window_->setCurrentPage("components"));
        QCOMPARE(input->text(),QString("校园数据"));
        QCOMPARE(progress->value(),63);
        QCOMPARE(window_->findChild<QProgressBar*>("demoProgress")->value(),63);
        auto* unknown = window_->findChild<QCheckBox*>("demoIndeterminate");
        unknown->setChecked(true);
        QCOMPARE(window_->findChild<QProgressBar*>("demoProgress")->maximum(),0);
        unknown->setChecked(false);
        QCOMPARE(window_->findChild<QProgressBar*>("demoProgress")->value(),63);
    }

    void disabledRoutesDoNotChangePage() {
        QVERIFY(!window_->setCurrentPage("map"));
        QVERIFY(!window_->setCurrentPage("unknown"));
        QCOMPARE(window_->currentPageId(),QString("import"));
        QVERIFY(!window_->findChild<QPushButton*>("nav_map")->isEnabled());
        QVERIFY(!window_->findChild<QPushButton*>("startImportButton")->isEnabled());
    }

    void selectedFileSurvivesThemeAndInvalidSelection() {
        QTemporaryFile file;
        QVERIFY(file.open()); file.write("unparsed fixture"); file.flush();
        auto* landing = window_->findChild<p1::app::ImportLanding*>();
        QVERIFY(landing);
        landing->selectFile(file.fileName());
        QCOMPARE(landing->selectedFile(),file.fileName());
        theme_->setDark(true);
        window_->setCurrentPage("settings"); window_->setCurrentPage("import");
        QCOMPARE(landing->selectedFile(),file.fileName());
        landing->selectFile(file.fileName()+".missing");
        QCOMPARE(landing->selectedFile(),file.fileName());
        QVERIFY(!window_->findChild<QPushButton*>("startImportButton")->isEnabled());
    }

    void pageReplacementTransfersOwnership() {
        auto* stack = window_->findChild<QStackedWidget*>("pageStack");
        QPointer<QWidget> oldPage = stack->currentWidget();
        auto* replacement = new QWidget;
        QVERIFY(window_->registerPage("import","已接入页面","测试模块替换",replacement));
        QCOMPARE(stack->currentWidget(),replacement);
        QCOMPARE(stack->count(),3);
        QCoreApplication::sendPostedEvents(nullptr,QEvent::DeferredDelete);
        QVERIFY(oldPage.isNull());
        QVERIFY(window_->registerPage("map","地图模块","测试注册",new QWidget));
        QVERIFY(window_->findChild<QPushButton*>("nav_map")->isEnabled());
        QVERIFY(window_->setCurrentPage("map"));
        QWidget rejected;
        QVERIFY(!window_->registerPage("unknown","","",&rejected));
        QVERIFY(!window_->registerPage("import","","",nullptr));
    }

    void compactLayoutAndKeyboardDialog() {
        window_->resize(1024,720);
        QTRY_VERIFY(window_->isCompact());
        QCOMPARE(window_->size(),QSize(1024,720));
        auto* choose = window_->findChild<QPushButton*>("chooseFileButton");
        QVERIFY(choose->isVisible());
        const QRect viewport(QPoint(0,0),window_->size());
        QVERIFY(viewport.contains(QRect(choose->mapTo(window_,QPoint(0,0)),choose->size())));
        QVERIFY(window_->findChild<QPushButton*>("nav_import")->text().isEmpty());
        QCOMPARE(window_->findChild<QPushButton*>("nav_import")->accessibleName(),QString("数据导入"));
        window_->setCurrentPage("components");
        QTest::mouseClick(window_->findChild<QPushButton*>("demoDialogButton"),Qt::LeftButton);
        auto* dialog = window_->findChild<QDialog*>("demoDialog");
        QVERIFY(dialog); QVERIFY(dialog->isVisible());
        QTest::keyClick(dialog,Qt::Key_Escape);
        QTRY_VERIFY(window_->findChild<QDialog*>("demoDialog")==nullptr);
        window_->resize(1440,900);
        QTRY_VERIFY(!window_->isCompact());
        QCOMPARE(window_->findChild<QPushButton*>("nav_import")->text(),QString("数据导入"));
    }
private:
    p1::ui::Theme* theme_ = nullptr;
    p1::app::MainWindow* window_ = nullptr;
};
QTEST_MAIN(UiTests)
#include "UiTests.moc"
