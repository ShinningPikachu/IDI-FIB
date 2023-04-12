#include <QLabel>
#include <QWidget>
#include <QKeyEvent>

class MyLabel:public QLabel
{
	Q_OBJECT //IMPORTANT
	public:
		MyLabel(QWidget *parent);

	public slots: //IMPORTANT
		void setTextMajuscules(QString);
        void ChangeNotaParcial(double);
        void ChangeNotaFinal(double);
        void ChangeNotaLab(double);
        void ChangeEntregaMenysMes(int);
        void Reset();
    signals:
        void setNotaParcial(double);
        void setNotaFinal(double);
        void setNotaLab(double);
        void setEntrega1(bool);
        void setEntrega2(bool);
        void setEntrega3(bool);
        void setEntrega4(bool);
        void sentmarkneeded(double);

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;
        virtual void keyReleaseEvent(QKeyEvent *event) override;
    private:
        double calcNota();
};
