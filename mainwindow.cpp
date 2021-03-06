//classes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "corona.h"
#include "gamemanagement.h"
#include "life.h"
#include "countvirus.h"
#include "disinfect.h"
#include "wash.h"
#include "patience.h"

//qt libraries
#include <QTime>
#include <QTimer>
#include <QMovie>
#include <QMediaPlayer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * view = ui->graphicsView;
    QGraphicsView * view2 = ui->graphicsView_2;

    // scene is a QGraphicsScene pointer field of the PlotWindow class
    // this makes our lives easier by letting us easily access it
    // from other functions in this class.
    scene_ = new QGraphicsScene;
    managementScene_ = new QGraphicsScene;

    //setting rectangle  boudnaries for hte two scenes
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    view->setScene(scene_);
    view2->setSceneRect(0,0,view2->frameSize().width(),view2->frameSize().height());
    view2->setScene(managementScene_);

    //setting height and width of graphics views
    height_ = view->frameSize().height();
    width_ = view->frameSize().width();
    qDebug() << "height_ is "<<height_<<"width_ is "<< width_;

    this->setStyleSheet("background-color: white;");
    QPixmap qp_title(":/assets/img/title_image.png");
    QGraphicsPixmapItem *title = scene_->addPixmap( qp_title );
    title->setPos(200,0);


//    TODO LATER
//    QPixmap qp_gif(":/assets/img/dancing_wario.gif");
//    QGraphicsPixmapItem * wario = scene_->addPixmap( qp_gif );
//    wario->setPos(40,0);

    //Warioware music playing the whole game
    QMediaPlayer * sound = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:/assets/sound/WarioWare, Inc. Super Smash Bros. Ultimate.mp3"));
    sound->setVolume(5);
    sound->play();


    Corona * c1 = new Corona(400,100);
    Corona * c2 = new Corona(400,200);
    scene_->addItem(c1);
    scene_->addItem(c2);

    connect(c1, &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);
    connect(c2, &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);

    MainMenu();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowCountdownTimerSlot(){
    GameManagement *board = new GameManagement();
    if (repeat_ == -1){
        repeat_ = 5;
        if(beat_){
            PhasePassed();
        }else{
            PhaseFailed();
        }
    }
    Timer *time = board->GetTimer(repeat_);
    managementScene_->addItem(time);
    repeat_--;
}

void MainWindow::CoronaSelectedSlot(Corona * c){
    scene_->removeItem(c);
    corona_num_--; //remove the corona counter for each cell clicked and removed
    QMediaPlayer * sound = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:/assets/sound/Corona/scream.wav"));
    sound->play();
}

void MainWindow::Games(){
    scene_->clear();
    ui->correct_label->setVisible(false);
    ui->incorrect_label->setVisible(false);
    int val = 0;
    val = qrand() % 4;
    switch(val){
        case 0 : {
                DisinfectGame();
                break;
            }
        case 1 :{
                PatienceGame();
                break;
            }
        case 2:{
                CountGame();
                break;
        }
        case 3:{
                WashGame();
                break;
        }
   }
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->close();
    ui->wario_gif->close();
    connect(time_,&QTimer::timeout,this,&MainWindow::ShowCountdownTimerSlot);
    time_->start(1000);
    if(lives_ != 0){
      this->Games();
        //this->ShowScore();
    }

}

void MainWindow::PhasePassed(){
    scene_->clear();
    ui->correct_label->setVisible(true);
    managementScene_->clear();
    GameManagement *board = new GameManagement();
    managementScene_->addItem(board);
    QMediaPlayer * sound2 = new QMediaPlayer();
    QMediaPlayer * sound3 = new QMediaPlayer();
    ui->prompt_label->setVisible(false);
    ui->countradio1->setVisible(false);
    ui->countradio2->setVisible(false);
    ui->countradio3->setVisible(false);
    ui->handwashing_gif->close();
    sound2->setMedia(QUrl("qrc:/assets/sound/Correct/correct.wav"));
    sound3->setMedia(QUrl("qrc:/assets/sound/Correct/cheering.wav"));
    sound2->play();
    sound3->play();
    score_++;
    score_text_ = "Score: "+ std::to_string(score_);
    QString qscore(score_text_.c_str());
    ui->score_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->score_label->setText(qscore);
    ui->continue_button->setVisible(true);
    if (lives_ == 1){
        Life *life1 = board->GetLife1();
        managementScene_->addItem(life1);
    }else if(lives_ == 2){
        Life *life1 = board->GetLife1();
        managementScene_->addItem(life1);
        Life *life2 = board->GetLife2();
        managementScene_->addItem(life2);
    }
    update();
    time_->stop();

}

void MainWindow::PhaseFailed(){
    scene_->clear();
    managementScene_->clear();
    GameManagement *board = new GameManagement();
    managementScene_->addItem(board);
    board->SetLife(-1);
    lives_ --;
    qDebug() << lives_;
    ui->prompt_label->setVisible(false);
    ui->countradio1->setVisible(false);
    ui->countradio2->setVisible(false);
    ui->countradio3->setVisible(false);
    ui->incorrect_label->setVisible(true);
    ui->continue_button->setVisible(true);
    ui->handwashing_gif->close();
    QMediaPlayer * sound = new QMediaPlayer();
    QMediaPlayer * sound2 = new QMediaPlayer();
    QMediaPlayer * sound3 = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:/assets/sound/Incorrect/booing.wav"));
    sound2->setMedia(QUrl("qrc:/assets/sound/Incorrect/incorrect.wav"));
    sound3->setMedia(QUrl("qrc:/assets/sound/Incorrect/lose.wav"));
    sound->play();
    sound2->play();
    sound3->play();
    if (lives_ == 1){
        Life *life1 = board->GetLife1();
        managementScene_->addItem(life1);
    }else if(lives_ == 2){
        Life *life1 = board->GetLife1();
        managementScene_->addItem(life1);
        Life *life2 = board->GetLife2();
        managementScene_->addItem(life2);
    }else{
        ShowScore();
    }
    qDebug()<< "Lives is, "<<lives_;
    update();
    time_->stop();
}

void MainWindow::PatienceGame(){
    scene_->clear();
    Patience *patience_game = new Patience(width_,height_);
    std::string prompt = "Patience.... ";
    QString qprompt(prompt.c_str());
    ui->prompt_label->setStyleSheet("background-color:rgba(255,255,255,0%)");
    ui->prompt_label->setVisible(true);
    ui->prompt_label->setText(qprompt);
    connect(patience_game, &Patience::LostTheMiniGame, this, &MainWindow::PatienceSelectedSlot);
    scene_->addItem(patience_game);
    beat_ = true;
}

void MainWindow::PatienceSelectedSlot(Patience * p){
    scene_->clear();
    std::string prompt = "You Weren't Patient....";
    QString qprompt(prompt.c_str());
    ui->prompt_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->prompt_label->setVisible(true);
    ui->prompt_label->setText(qprompt);
    QImage *img = new QImage(":/assets/img/cryguy.jpg");
    *img = img->scaled(width_,height_);
    QBrush bg_brush(*img);
    scene_ ->setBackgroundBrush(bg_brush);
    beat_ = false;
}

void MainWindow::WashGame(){
    scene_->clear();
    Wash *wash_game = new Wash(width_,height_);
    scene_->addItem(wash_game);
    ui->handwashing_gif->setVisible(true);
    ui->handwashing_gif->setStyleSheet("background-color:rgba(213,240,172,100%)");
    RandomHelper();
    QMovie *hands_washing = new QMovie(":/assets/img/handwashing.gif");
    ui->handwashing_gif->setMovie(hands_washing);
    hands_washing->start();

    std::string prompt = "CLICK! Wash Away The Germs!";
    QString qprompt(prompt.c_str());
    ui->prompt_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->prompt_label->setVisible(true);
    ui->prompt_label->setText(qprompt);
    connect(wash_game, &Wash::WashSelected, this, &MainWindow::WashSelectedSlot);
}

void MainWindow::RandomHelper(){
    amount_ = qrand() % 20 + 10;
}

void MainWindow::WashSelectedSlot(Wash * w){
    if (amount_ > 0){
        QMediaPlayer * sound = new QMediaPlayer();
        sound->setMedia(QUrl("qrc:/assets/sound/wash hand minigame/water.wav"));
        sound->play();
        beat_ = false;
    }else{
      beat_ = true;
    }
    amount_--;
}

void MainWindow::MainMenu(){
    //creating the game management
    GameManagement *board = new GameManagement();
    QMovie *wario_dance = new QMovie(":/assets/img/wario_dancing.gif");
    ui->wario_gif->setMovie(wario_dance);
    wario_dance->start();
    qsrand(static_cast<unsigned>(QTime::currentTime().msec()));

    QImage *img = new QImage(":/assets/img/gamemanagement_background.jpeg");
    QBrush bg_brush(*img);
    managementScene_ ->setBackgroundBrush(bg_brush);

    //Game management Overhead Labels
    score_text_ = "Score: "+ std::to_string(score_);
    QString qscore(score_text_.c_str());
    ui->score_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->time_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->life_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->score_label->setText(qscore);
    ui->handwashing_gif->setVisible(false);
    ui->prompt_label->setVisible(false);
    ui->correct_label->setVisible(false);
    ui->incorrect_label->setVisible(false);
    ui->continue_button->setVisible(false);
    ui->good_job->setVisible(false);
    ui->score_big->setVisible(false);
    Life *life1 = board->GetLife1();
    managementScene_->addItem(life1);
    Life *life2 = board->GetLife2();
    managementScene_->addItem(life2);
    lives_ = board->GetLives();
    ui->countradio1->setVisible(false);
    ui->countradio2->setVisible(false);
    ui->countradio3->setVisible(false);
}

void MainWindow::DisinfectGame(){
    scene_->clear();
    std::string prompt = "Click All the Viruses!";
    QString qprompt(prompt.c_str());
    ui->prompt_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->prompt_label->setVisible(true);
    ui->prompt_label->setText(qprompt);
    Disinfect *disinfect = new Disinfect(width_, height_);
    connect(disinfect, &Disinfect::LostTheMiniGame, this, &MainWindow::DisinfectGameSlot);
    scene_->addItem(disinfect);
    std::vector<Corona *> cells= disinfect->get_cells();
    for(uint i=0; i< cells.size(); i++){
        scene_->addItem(cells[i]);
        connect(cells[i], &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);

    }
    corona_num_=cells.size();
}

void MainWindow::DisinfectGameSlot(Disinfect * d){

    if (corona_num_ == 0){
        std::string prompt = "All Viruses Cleansed!";
        QString qprompt(prompt.c_str());
        ui->prompt_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
        ui->prompt_label->setVisible(true);
        ui->prompt_label->setText(qprompt);
        beat_=true;
    }
    else{
        beat_=false;
    }
}

void MainWindow::on_continue_button_clicked()
{
    ui->continue_button->close();
    time_->start(1000);
    Games();
}

void MainWindow::ShowScore(){
    scene_->clear();
    Score * val;
    ui->incorrect_label->setVisible(false);
    ui->correct_label->setVisible(false);
    ui->continue_button->setVisible(false);
    if(score_>=10){
        val= ScoreFactory::GetGood(width_, height_);
        QMovie * dance= val->get_wariodance();
        ui->wario_gif->setMovie(dance);
        dance->start();
        ui->wario_gif->show();
        QColor color= val->get_color();
        scene_->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
        QMediaPlayer * sound= val->get_sound();
        sound->play();
        QMediaPlayer * song= val->get_song();
        song->play();
        std::string prompt = "Good Job!!";
        QString qprompt(prompt.c_str());
        ui->good_job->setStyleSheet("background-color:rgba(0,0,0,0%)");
        ui->good_job->setVisible(true);
        ui->good_job->setText(qprompt);

    }
    else if(score_<=3){
        val= ScoreFactory::GetBad(width_, height_);
        QColor color= val->get_color();
        scene_->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
        QMediaPlayer * sound= val->get_sound();
        sound->play();
        QMediaPlayer * song= val->get_song();
        song->play();
        std::vector<Corona *> cells=val->get_cells();
        for(uint i=0; i< cells.size(); i++){
            scene_->addItem(cells[i]);
            connect(cells[i], &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);

        }
        std::string prompt = "FAIL :(";
        QString qprompt(prompt.c_str());
        ui->good_job->setStyleSheet("background-color:rgba(0,0,0,0%)");
        ui->good_job->setVisible(true);
        ui->good_job->setText(qprompt);
    }
    else{
        val= ScoreFactory::GetMeh(width_, height_);
        QColor color= val->get_color();
        scene_->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
        QMediaPlayer * sound= val->get_sound();
        sound->play();
        std::vector<Corona *> cells=val->get_cells();
        for(uint i=0; i< cells.size(); i++){
            scene_->addItem(cells[i]);
            connect(cells[i], &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);

        }
        std::string prompt = "Not Bad...";
        QString qprompt(prompt.c_str());
        ui->good_job->setStyleSheet("background-color:rgba(0,0,0,0%)");
        ui->good_job->setVisible(true);
        ui->good_job->setText(qprompt);

    }
    std::string score = "Score: "+std::to_string(score_);
    QString qscore(score.c_str());
    ui->score_big->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->score_big->setVisible(true);
    ui->score_big->setText(qscore);




}

void MainWindow::CountGame(){
    scene_->clear();
    ui->incorrect_label->setVisible(false);
    ui->correct_label->setVisible(false);

    std::string prompt = "How many are there?";
    QString qprompt(prompt.c_str());
    ui->prompt_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    ui->prompt_label->setVisible(true);
    ui->prompt_label->setText(qprompt);
    CountVirus *count_virus = new CountVirus(width_, height_);
    scene_->addItem(count_virus);
    count_virus->populate(width_,height_);
    std::vector<Corona *> cells= count_virus->get_cells();
    for(uint i=0; i< cells.size(); i++){
        scene_->addItem(cells[i]);
        //connect(cells[i], &Corona::DeleteCell, this, &MainWindow::CoronaSelectedSlot);
    }
    corona_num_=cells.size();
    int temp_holder = corona_num_;
    ui->countradio1->setVisible(true);
    ui->countradio2->setVisible(true);
    ui->countradio3->setVisible(true);

    std::string answer1;
    std::string answer2;
    std::string answer3;

    answer1 = std::to_string(corona_num_);
    answer2 = std::to_string(temp_holder+2);
    answer3 = std::to_string(temp_holder-1);

    QString qanswer1(answer1.c_str());
    ui->countradio1->setText(qanswer1);
    QString qanswer2(answer2.c_str());
    ui->countradio2->setText(qanswer2);
    QString qanswer3(answer3.c_str());
    ui->countradio3->setText(qanswer3);

    this->update();
}

void MainWindow::on_countradio1_clicked()
{
    beat_ = true;
}

void MainWindow::on_countradio2_clicked()
{
    beat_=false;
}

void MainWindow::on_countradio3_clicked()
{
    beat_=false;
}
