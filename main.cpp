#include <QCoreApplication>
#include <QSerialPort>
#include <QDebug>
#include <QDateTime>
#include <unistd.h>
#include <signal.h>

QSerialPort serialOut;

void sig_handler(int);

int main(int argc, char *argv[])
{
  //  qDebug()<<"ucuy";


    QCoreApplication a(argc, argv);

    signal(SIGTERM, sig_handler);

    serialOut.setPortName("/dev/pts/1");
    serialOut.setBaudRate(QSerialPort::Baud9600);
    serialOut.setDataBits(QSerialPort::Data8);
    serialOut.setStopBits(QSerialPort::OneStop);
    serialOut.setFlowControl(QSerialPort::NoFlowControl);
    serialOut.setParity(QSerialPort::NoParity);

    if(!serialOut.open(QIODevice::WriteOnly))
    {
        qDebug()<<Q_FUNC_INFO<<"serial open fail"<<serialOut.errorString();
        return 1;
    }


    const QString nmeaData =
            "PRMB,A,,,,,,,,,,,,,A*4A\r\n"
            "$GPGGA,032558.62,0711.9253,S,11244.3461,E,1,11,01,+0031,M,+018,M,,*50\r\n"
            "$GPRMC,032558.62,A,0711.9253,S,11244.3461,E,000.0,254.4,080219,01.,E,A*29\r\n"
            "$GPVTG,254.4,T,254.4,M,000.0,N,000.0,K,A*23\r\n"
            "$GPZDA,032600.00,08,02,2019,00,00*61\r\n"
            "$GPFPD,2039,443810.480,100.088,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*74\r\n"
            "$GPDTM,W84,,0.0,N,0.0,E,+0.0,W84*44\r\n"
            "$GPAPB,A,A,,,N,,,,T,,,T,,,A*67\r\n"
            "$GPRMB,A,,,,,,,,,,,,,A*4A\r\n"
            "$IIMWV,044.5,R,15.4,N,A*08\r\n"
            "$IIMWV,068,T,11.6,N,A*1D\r\n"
            "$GPFPD,2039,443810.480,70.080,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*4A\r\n"
            "$IIMWV,045.5,R,16.2,N,A*0C\r\n"
            "$IIMWV,066.5,T,12.5,N,A*08\r\n"
            "$IIVHW,,T,105,M,6.6,N,12.2,K*50\r\n"
            "$GPFPD,2039,443810.480,215.278,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*7E\r\n"
            "$IIVHW,,T,105.5,M,7.4,N,13.7,K*4C\r\n"
            "$IIVHW,,T,107.5,M,7.4,N,13.7,K*4E\r\n"
            "$HEHDT,341.6,T*2F\r\n"
            "$HEHDT,342.3,T*29\r\n"
            "$GPFPD,2039,443810.480,348.276,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*79\r\n"
            "$HEHDT,343.0,T*2B\r\n"
            "$IIXDR,C,19.52,C,TempAir*19\r\n"
            "$GPFPD,2039,443810.480,359.046,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*78\r\n"
            "$IIXDR,P,1.02481,B,Barometer*29\r\n"
            "$GPFPD,2039,443810.480,300.075,0.340,1.188,-7.1986473,112.7392174,42.45,0.004,-0.004,0.011,2.923,15,19,04*74\r\n"
            "$WIXDR,P,1008.8,B,3,C,31.9,C,0,H,67.3,P,2,C,99.9,C,1*5B\r\n"
            "$IIMWD,020.0,T,022.0,M,8.8,N,4.5,M*47\r\n"
            "$GPFPD,2039,443810.280,258.047,0.342,1.188,-7.1986474,112.7392174,42.42,0.004,-0.003,0.010,2.925,15,19,04*7D\r\n"

            "$RATTM,85,3.34,330.7,T,3.6,180.1,T,1.64,47.8,N,,T,,211119,A*19\r\n"
            "$RATTM,84,2.36,57.7,T,1.8,139.6,T,2.34,-10.9,N,,T,,211119,A*08\r\n"
            "$RATTM,83,2.19,350.9,T,2.0,101.4,T,2.05,23.3,N,,T,,211119,A*1F\r\n"
            "$RATTM,82,2.84,50.4,T,1.8,105.7,T,2.35,-53.2,N,,T,,211117,A*0E\r\n"
            ;

  //  qDebug()<<Q_FUNC_INFO<<"nmeaData size"<<nmeaData.size();

    int nmeaDataIndx = 0;
    const int MAX = 100;
    const int MIN = 1;

    while (1)
    {
        int range = MAX - MIN + 1;
//        int num = 5;
        int num = rand() % range + MIN;

   //     qDebug()<<Q_FUNC_INFO<<"num random"<<num;


        if((num+nmeaDataIndx) >= (nmeaData.size()-1))
        {
            num = nmeaData.size() - nmeaDataIndx;
  //         qDebug()<<Q_FUNC_INFO<<"num exceed"<<num<<nmeaDataIndx;
        }


        QString data_send = nmeaData.mid(nmeaDataIndx,num);
 //       qDebug()<<Q_FUNC_INFO<<"send data"<<data_send;
        serialOut.write(data_send.toUtf8());
        serialOut.waitForBytesWritten(10);

        nmeaDataIndx += num;

        if(nmeaDataIndx >= (nmeaData.size()-1))
            nmeaDataIndx = 0;

        usleep(1000000); //1 second

    }


    return a.exec();
}

void sig_handler(int sig)
{
    switch (sig)
    {
    case SIGTERM:
        qDebug()<<Q_FUNC_INFO<<"SIGTERM";
        if(serialOut.isOpen())
            serialOut.close();
        abort();
    default:
        qDebug()<<Q_FUNC_INFO<<"default";
        abort();
    }
}
