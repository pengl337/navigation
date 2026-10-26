#include "app/ImportLanding.h"
#include "ui/common/Components.h"
#include "ui/common/Theme.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QStyle>

namespace p1::app {
using namespace ui;
ImportLanding::ImportLanding(Theme& theme, QWidget* parent) : QWidget(parent) {
    auto* columns = new QHBoxLayout(this);
    columns->setContentsMargins(0,0,0,0); columns->setSpacing(24);
    auto* leftContent = new QWidget;
    auto* left = new QVBoxLayout(leftContent);
    left->setContentsMargins(0,0,6,12); left->setSpacing(24);
    picker_ = new Card(theme);
    picker_->setObjectName("filePickerCard");
    auto* body = picker_->body();
    body->addWidget(label("01  /  添加数据", "eyebrow"));
    body->addWidget(label("从本地开始", "sectionTitle"));
    body->addWidget(label("选择路网、兴趣点或轨迹文件，为离线导航准备数据。", "secondary"));
    auto* illustration = new QLabel;
    illustration->setAlignment(Qt::AlignCenter);
    illustration->setMinimumHeight(96);
    auto updateIcon = [illustration,&theme] { illustration->setPixmap(theme.icon("upload").pixmap(56,56)); };
    updateIcon(); connect(&theme,&Theme::changed,this,updateIcon);
    body->addWidget(illustration);
    fileName_ = label("尚未选择文件", "cardTitle");
    fileName_->setObjectName("selectedFileName");
    fileInfo_ = label("文件将保留在本机。当前仅选择文件，解析服务尚未接入。", "secondary");
    body->addWidget(fileName_); body->addWidget(fileInfo_);
    auto* choose = button("选择文件",true);
    choose->setObjectName("chooseFileButton");
    connect(choose,&QPushButton::clicked,this,[this] {
        // No file extension assumptions before member 2 confirms the data format.
        const auto path = QFileDialog::getOpenFileName(this,"选择本地数据文件",{},"所有文件 (*)");
        if (!path.isEmpty()) selectFile(path);
    });
    body->addWidget(choose);
    auto* start = button("开始导入");
    start->setObjectName("startImportButton"); start->setEnabled(false);
    start->setToolTip("等待 2 号接入解析与存储服务");
    body->addWidget(start);
    body->addWidget(badge("!  解析服务待接入","warning"));
    left->addWidget(picker_);
    auto* guide = new Card(theme);
    guide->body()->addWidget(label("导入流程", "cardTitle"));
    guide->body()->addWidget(label("1   选择文件\n\n2   后台解析与保存\n\n3   查看统计与数据列表", "secondary"));
    left->addWidget(guide); left->addStretch();
    auto* leftScroll = scrollable(leftContent);
    leftScroll->setObjectName("importTaskPanel");
    columns->addWidget(leftScroll);

    auto* rightContent = new QWidget;
    auto* right = new QVBoxLayout(rightContent);
    right->setContentsMargins(6,0,6,12); right->setSpacing(24);
    auto* stats = new QHBoxLayout; stats->setSpacing(16);
    const QStringList titles{"数据集", "有效记录", "异常记录"};
    for (int i=0;i<titles.size();++i) {
        auto* card = new Card(theme);
        card->body()->setContentsMargins(16,16,16,16);
        card->body()->setSpacing(8);
        card->body()->addWidget(label(titles[i],"secondary"));
        card->body()->addWidget(label(i == 0 ? "0" : "—", "metric"));
        card->body()->addWidget(label(i == 0 ? "本次会话" : "等待解析", "caption"));
        stats->addWidget(card);
    }
    right->addLayout(stats);
    auto* empty = new Card(theme);
    empty->body()->addWidget(label("本地数据集", "sectionTitle"));
    auto* tableHeader = new QHBoxLayout;
    for (const auto& column : {"名称", "类型", "记录数", "状态"})
        tableHeader->addWidget(label(QString::fromUtf8(column), "caption"),1);
    empty->body()->addLayout(tableHeader);
    empty->body()->addStretch();
    auto* folder = new QLabel;
    folder->setAlignment(Qt::AlignCenter);
    auto updateFolder = [folder,&theme] { folder->setPixmap(theme.icon("folder").pixmap(64,64)); };
    updateFolder(); connect(&theme,&Theme::changed,this,updateFolder);
    empty->body()->addWidget(folder);
    auto* emptyTitle = label("你的地图，从一份数据开始", "sectionTitle");
    emptyTitle->setAlignment(Qt::AlignCenter);
    empty->body()->addWidget(emptyTitle);
    auto* emptyHint = label("选择左侧文件，准备导入。解析与保存接入后，数据集将显示在这里。", "secondary");
    emptyHint->setAlignment(Qt::AlignCenter);
    empty->body()->addWidget(emptyHint);
    empty->body()->addStretch();
    empty->setMinimumHeight(300);
    right->addWidget(empty,1);
    right->addWidget(label("离线优先  ·  本地文件  ·  数据来源清楚可查", "caption"));
    columns->addWidget(scrollable(rightContent),1);
}

void ImportLanding::selectFile(const QString& path) {
    const QFileInfo info(path);
    if (!info.exists() || !info.isFile() || !info.isReadable()) {
        fileInfo_->setText("无法读取所选文件，请检查路径与文件权限后重新选择。");
        return; // Keep any previous valid selection intact.
    }
    selectedFile_ = info.absoluteFilePath();
    fileName_->setText(info.fileName());
    fileName_->setToolTip(info.absoluteFilePath());
    fileInfo_->setText(QString("%1 字节 · 已选择，尚未解析或保存。").arg(info.size()));
}

void ImportLanding::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    layout()->setSpacing(window()->width() < 1280 ? 16 : 24);
    findChild<QWidget*>("importTaskPanel")->setFixedWidth(window()->width() < 1280 ? 304 : 360);
}
}
