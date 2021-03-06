#include "mainwindow.h"

QToolBar* MainWindow::createMainToolBar()
{
    QToolBar* bar = new QToolBar("Main ToolBar");

    bar->addAction(QPixmap(":/img/Toolbar/newfile.ico"), "Создать файл", this, SLOT(CreateNewFile()));
    bar->addAction(QPixmap(":/img/Toolbar/openfile.ico"), "Открыть файл", this, SLOT(OpenFile()));

    bar->setMovable(false);
    bar->setIconSize(QSize(25,25));

    return bar;
}

QToolBar* MainWindow::createWorkToolBar()
{
    QToolBar* bar = new QToolBar("Work ToolBar");

    bar->addAction(QPixmap(":img/Toolbar/save.ico"), "Сохранить", this, SLOT(SaveFile()));
    bar->addAction(QPixmap(":img/Toolbar/saveas.ico"), "Сохранить как", this, SLOT(SaveFileAs()));

    bar->addSeparator();

    bar->addAction(QPixmap(":/img/Toolbar/topdf.ico"), "Экспорт в pdf", this, SLOT(ExportToPDF()));
    bar->addAction(QPixmap(":/img/Toolbar/tohtml.ico"), "Экспорт в html", this, SLOT(ExportHtml()));

    bar->setMovable(false);
    bar->setIconSize(QSize(25,25));
    return bar;
}

QToolBar* MainWindow::createExtraToolBar()
{
    QToolBar* bar = new QToolBar("Extra ToolBar");
    html_ico = bar->addAction(QPixmap("://img/Toolbar/htmlView.png"), "Предпросмотр Html", this, SLOT(HtmlCheck()));
    text_ico = bar->addAction(QPixmap(":/img/Toolbar/extra_txt.ico"), "Предпросмотр", this, SLOT(PreviewCheck()));
    markdown_ico = bar->addAction(QPixmap(":/img/Toolbar/extra_markdown.ico"), "Редактор Markdown", this, SLOT(MarkdowneditCheck()));

    markdown_ico->setDisabled(true);
    html_ico->setDisabled(true);
    text_ico->setDisabled(true);

    markdown_ico->setCheckable(true);
    html_ico->setCheckable(true);
    text_ico->setCheckable(true);

    bar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    bar->setMovable(false);
    bar->setIconSize(QSize(25,25));

    return bar;
}

void MainWindow::MarkdowneditCheck()
{
    MarkdowntextEdit->hide();
    if (markdown_ico->isChecked())
    {
        MarkdowntextEdit->show();
    }
}
void MainWindow::PreviewCheck()
{
    Preview->hide();
    if (text_ico->isChecked())
    {
        Preview->show();
        //in other words we give signal that we can convert info from markdown text edit to Preview
        emit MarkdowntextEdit->textChanged();
    }
}
void MainWindow::HtmlCheck()
{
    htmlPreview->hide();
    if (html_ico->isChecked())
    {
        htmlPreview->show();
        //in other words we give signal that we can convert info from markdown text edit to htmlPreview
        emit MarkdowntextEdit->textChanged();
    }
}

void MainWindow::CreateNewFile()
{
    if(justCreated && !isChanged)
        return;

    if (isChanged && (justCreated || file.isOpen()) && SaveDialog() == -1)
        return;

    markdown_ico->setDisabled(false);
    html_ico->setDisabled(false);
    text_ico->setDisabled(false);    
    WorkToolBar->setDisabled(false);
    TextToolBar->setDisabled(false);

    disableMenu(false);

    markdown_ico->setChecked(true);
    MarkdowntextEdit->clear();
    MarkdowntextEdit->show();
    cursorPosition->show();
    fileSize->show();

    setWindowTitle(QString("new - Markdown Editor"));

    isChanged = false;
    justCreated = true;
}

void MainWindow::OpenFile()
{
    if (isChanged && (justCreated || file.isOpen()) && SaveDialog() == -1)
        return;

    QString fileName = QFileDialog::getOpenFileName(0, "Открыть файл", "", "*.markdown");
    if(fileName.isEmpty())
    {
        qDebug() << "Read and write paths are empty";
        return;
    }
    if(file.isOpen())
        file.close();

    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while opening for writing and reading";
        return;
    }

    MarkdowntextEdit->setPlainText(file.readAll());

    markdown_ico->setDisabled(false);
    html_ico->setDisabled(false);
    text_ico->setDisabled(false);
    WorkToolBar->setDisabled(false);
    TextToolBar->setDisabled(false);
    disableMenu(false);
    markdown_ico->setChecked(true);
    MarkdowntextEdit->show();
    cursorPosition->show();
    fileSize->show();

    setWindowTitle(QString("%1 - Markdown Editor").arg(file.fileName()));

    isChanged = false;
    justCreated = false;
}

void MainWindow::SaveFile()
{
    if(!isChanged)
        return;

    if (justCreated)
    {
        SaveFileAs();
        return;
    }
    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error while opening for writing";
        return;
    }

    file.write(MarkdowntextEdit->toPlainText().toStdString().data());
    setWindowTitle(QString(windowTitle().remove(0,1)));
    file.close();
    file.open(QIODevice::ReadOnly);
    isChanged = false;
    justCreated = false;
}

int MainWindow::SaveFileAs() // 1 - saveas | 0 - no save | -1 - cancel
{
    if(justCreated && !isChanged)
        return -1;

    QString fileName = QFileDialog::getSaveFileName(0, "Сохранить файл как","", "*.markdown");
    if(fileName.isEmpty())
    {
        qDebug() << "Write path is empty";
        return -1;
    }
    file.close();
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error while opening for writing";
        return -1;
    }
    file.write(MarkdowntextEdit->toPlainText().toStdString().data());
    setWindowTitle(QString("%1 - Markdown Editor").arg(file.fileName()));
    file.close();
    file.open(QIODevice::ReadOnly);
    isChanged = false;
    justCreated = false;

    return 1;
}

void MainWindow::ExportToPDF()
{
     QString filePath = QFileDialog::getSaveFileName(0, "Экспорт в PDF","", "*.pdf");
     if(filePath.isEmpty())
     {
         qDebug() << "Read and write paths are empty";
         return;
     }
     if(Preview->isHidden())
     {
         Preview->setVisible(true);
         emit MarkdowntextEdit->textChanged();
     }
     QPrinter printer(QPrinter::HighResolution);
     printer.setOutputFormat(QPrinter::PdfFormat);
     printer.setOutputFileName(filePath);
     Preview->print(&printer);
}

void MainWindow::ExportHtml()
{
    QString fileName = QFileDialog::getSaveFileName(0, "Экспорт в HTML","", "*.html");

    if(fileName.isEmpty())
    {
        qDebug() << "Write path is empty";
        return;
    }
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error while opening for writing";
        return;
    }
    file.write(htmlPreview->toPlainText().toStdString().data());
    file.close();
}

void MainWindow::AboutProgram()
{
    QMessageBox msgBox;
    msgBox.setParent(0);
    msgBox.setWindowTitle("О программе - Markdown Editor");
    msgBox.setText("");
    QPixmap p;
    p.load(":/img/header.png");
    msgBox.setIconPixmap(p);
    msgBox.setWindowIcon(QIcon(":/img/window_ico.ico"));
    msgBox.exec();
}

void MainWindow::MarkdownHelp()
{
    QDesktopServices::openUrl(QUrl("https://www.markdownguide.org/"));
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    if (isChanged && (justCreated || file.isOpen()) && SaveDialog() == -1)
    {
        event->ignore();
        return;
    }
    if(file.isOpen())
        file.close();
    QMainWindow::closeEvent(event);
 }
