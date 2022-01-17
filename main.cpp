#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QColor>
#include <vector>

class MyMainWindow : public QMainWindow {

    public:

        bool action;
        int paintFlag;
        int x;
        int y;
        int x2;
        int y2;
        int i;
        static bool firstClick;
        static bool status;
        static bool status1;
        static int cnt;
        static int control;
        static int itrNumb;

        QLabel* MyLabel;
        QMenu* EditMenu;
        QAction* EditNoviTekst;
        QAction* EditNoviTekst1;
        QMenu* FileMenu;
        QAction* FileSaveAs;
        QAction* FileOpen;
        QPoint p1;
        QPoint p2;
        QVector <QPoint> vec;

        MyMainWindow();
        void FileSaveAsMenu();
        void FileOpenMenu();
        void EditNoviTekstMenu();
        void mousePressEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void changeValue();
};

int MyMainWindow :: control = 0;
int MyMainWindow :: cnt = 0;
int MyMainWindow :: itrNumb = 0;
bool MyMainWindow :: firstClick = true;
bool MyMainWindow :: status = true;
bool MyMainWindow :: status1 = true;

class MyDialog: public QDialog {

    public:

        MyDialog();
        QVBoxLayout* mainLayout;
        QWidget* editWidget;
        QFormLayout* editLayout;
        QLineEdit* lineEdit;
        QDialogButtonBox* buttonBox;
};

MyDialog::MyDialog() {

    lineEdit = new QLineEdit;
    editLayout = new QFormLayout;
    editWidget = new QWidget;
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout = new QVBoxLayout;


    editLayout->addRow(new QLabel(tr("New text:")), lineEdit);
    editWidget->setLayout(editLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(editWidget);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setWindowTitle("Change Your Text");
}

void MyMainWindow::EditNoviTekstMenu() {

    MyDialog dialog;
    int ret = dialog.exec();

    if (ret == QDialog::Accepted){

        MyLabel->setText(dialog.lineEdit->text());
    }
}

MyMainWindow::MyMainWindow() {

    MyLabel = new QLabel(this);
    MyLabel->setText("Insert: Ctrl + N");

    MyLabel->move(250, 250);

    EditNoviTekst = new QAction(tr ("&New Label"), this);
    EditNoviTekst->setShortcut(tr("CTRL+N"));

    EditNoviTekst1 = new QAction(tr ("&Change Status"), this);
    EditNoviTekst1->setShortcut(tr("K"));

    connect(EditNoviTekst, &QAction::triggered, this, &MyMainWindow::EditNoviTekstMenu);
    connect(EditNoviTekst1, &QAction::triggered, this, &MyMainWindow::changeValue);

    EditMenu = menuBar()->addMenu(tr("&Menu"));
    EditMenu->addAction(EditNoviTekst);
    EditMenu->addAction(EditNoviTekst1);

    FileSaveAs = new QAction(tr("&Save As..."), this);
    FileSaveAs->setShortcut(tr("CTRL+S"));
    connect(FileSaveAs, &QAction::triggered, this, &MyMainWindow::FileSaveAsMenu);

    FileOpen = new QAction(tr("&Open..."), this);
    FileOpen->setShortcut(tr("CTRL+O"));
    connect(FileOpen, &QAction::triggered, this, &MyMainWindow::FileOpenMenu);

    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(FileSaveAs);
    FileMenu->addAction(FileOpen);
}


void MyMainWindow::mousePressEvent(QMouseEvent* event) {

    paintFlag = 0;

    status = status1;

    if(status == true) {

        if(event->button()==Qt::LeftButton) {


            if(firstClick) {

                p1.setX(event->position().x());
                p1.setY(event->position().y());

                x = p1.x();
                y = p1.y();

                vec.push_back(p1);


                firstClick = false;
             }

            else if(!firstClick) {

                p2.setX(event->position().x());
                p2.setY(event->position().y());

                x2 = p2.x();
                y2 = p2.y();

                vec.push_back(p2);

                firstClick = true;
            }

             paintFlag = 1;

        }
    }

    else if(status == false) {

        if (event->button() == Qt::LeftButton)
            MyLabel->move(int(event->position().x()), int(event->position().y()));
    }
}

void MyMainWindow::paintEvent(QPaintEvent* event) {

    if(paintFlag == 1) {

        QPainter painter(this);
        QPen linepen(Qt::black);

        linepen.setWidth(4);

        painter.setPen(linepen);

        for(auto i = vec.begin() ; i != vec.end()-1 ; i++) {

            //if(vec.length() == 1)
               // painter.drawLine(i->x(), i->y() , i->x(), i->y());

         //  else
                painter.drawLine(i->x(), i->y() , (i+1)->x(), (i+1)->y());


        }

     }

    update();

}

void MyMainWindow::FileOpenMenu() {

    QString fileName = QFileDialog::getOpenFileName(this, "Open Geometry...", "", "OOP File (*.oop)");

    if (!fileName.isEmpty()) {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {

            QMessageBox::information(this, "Unable to Open File", file.errorString());
            return;
        }

        QTextStream in(&file);
        QString str; str = in.readLine();

        if(str=="oop file") {

            int x, y, w, h;
            in >> x >> y;

            str = in.readLine();
            MyLabel->setText(str);

            MyLabel->move(x, y);
            in >> x >> y >> w >> h;
            this->setGeometry(x, y, w, h);
        }
    }
}

void MyMainWindow::FileSaveAsMenu() {

    QString fileName = QFileDialog::getSaveFileName(this, "Save As...", "", "OOP File (*.oop)");

    if (!fileName.isEmpty()) {

        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {

            QMessageBox::information(this, "Unable to Open File", file.errorString());
            return;
        }

        QTextStream out(&file);
        out << "oop file" << Qt::endl;
        out << MyLabel->text() << Qt::endl;
        out << MyLabel->pos().x() << Qt::endl;
        out << MyLabel->pos().y() << Qt::endl;
        out << pos().x() << Qt::endl;
        out << pos().y() << Qt::endl;
        out << size().width() << Qt::endl;
        out << size().height() << Qt::endl;
    }
}

void MyMainWindow::keyPressEvent(QKeyEvent *event) {


}

void MyMainWindow::changeValue() {

    if(status1 == true)
        status1 = false;

    else
        status1 = true;
}

int main(int argc, char *argv[]) {

    QApplication app (argc, argv);

    MainWindow w;
    MyMainWindow mainWindow;

    srand(time(NULL));

    mainWindow.resize(1000, 800);

    mainWindow.show();

    return app.exec();
}
