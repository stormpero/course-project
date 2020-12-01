#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set title & ico
    setWindowTitle("Markdown Editor");
    setWindowIcon(QIcon(":/img/window_ico.ico"));

    //Create Texteditors & Textviews
    MarkdowntextEdit = new QTextEdit;
    TextPreview = new QTextEdit;
    htmlPreview = new QTextEdit;

    TextPreview->setReadOnly(true);
    htmlPreview->setReadOnly(true);
    ui->boxl->addWidget(MarkdowntextEdit);
    ui->boxl->addWidget(TextPreview);
    ui->boxl->addWidget(htmlPreview);
    MarkdowntextEdit->hide();
    TextPreview->hide();
    htmlPreview->hide();

    //Create Toolbar
    addToolBar(Qt::TopToolBarArea,createMainToolBar());
    WorkToolBar = createWorkToolBar();
    addToolBar(Qt::TopToolBarArea,WorkToolBar);
    addToolBar(Qt::TopToolBarArea,createExtraToolBar());
    WorkToolBar->setDisabled(true);
    connect(MarkdowntextEdit, &QTextEdit::textChanged, [this]() { isChanged = true; TextPreview->setMarkdown(MarkdowntextEdit->document()->toRawText());});
}

MainWindow::~MainWindow()
{
    delete ui;
}

