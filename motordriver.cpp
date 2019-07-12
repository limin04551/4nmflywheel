#include "motordriver.h"

MotorDriver::MotorDriver(QObject *parent) : QObject(parent)
  ,isInit(false)
{
    spd_array_.spd = 0;
    tor_array_.torque = 0;
}

bool MotorDriver::init(QString port_name,QString baud_rate)
{
    if (!serial_port_.isOpen()){

        serial_port_.setPortName(port_name);
        serial_port_.setBaudRate(baud_rate.toInt());
        serial_port_.setParity(QSerialPort::OddParity);


        if(!serial_port_.open(QIODevice::ReadWrite)){

            emit sendErrText(serial_port_.errorString());
            return false;

        }

    }
    else{

    }

    isInit = true;
    return true;
}

QByteArray MotorDriver::calSpdData(QString spd)
{
    static QByteArray spd_arr;
    spd_array_.spd = spd.toInt() * 2;
    if (spd_arr.isEmpty()){
        spd_arr.resize(7);
        spd_arr[0] = 0xD3;
        spd_arr[1] = 0x00;
        spd_arr[2] = 0x00;
        spd_arr[3] = spd_array_.array[1];
        spd_arr[4] = spd_array_.array[0];
        spd_arr[5] = 0x5B;
        // cal total
        uchar t = 0x00;
        for (uint i = 0; i < 6;++i){
            t += spd_arr[i];
        }
        spd_arr[6] = t;

    }
    else{
        spd_arr[3] = spd_array_.array[1];
        spd_arr[4] = spd_array_.array[0];
        uchar t = 0x00;
        for (uint i = 0; i < 6;++i){
            t += spd_arr[i];
        }
        spd_arr[6] = t;
    }
    return spd_arr;
}

QByteArray MotorDriver::calTorData(QString tor)
{
    static QByteArray tor_arr;
    tor_array_.torque = tor.toInt() / 0.058;
    if (tor_arr.isEmpty()){
        tor_arr.resize(7);
        tor_arr[0] = 0x1C;
        tor_arr[1] = 0x00;
        tor_arr[2] = 0x00;
        tor_arr[3] = tor_array_.array[1];
        tor_arr[4] = tor_array_.array[0];
        tor_arr[5] = 0x5B;
        // cal total
        uchar t = 0x00;
        for (uint i = 0; i < 6;++i){
            t += tor_arr[i];
        }
        tor_arr[6] = t;

    }
    else{
        tor_arr[3] = tor_array_.array[1];
        tor_arr[4] = tor_array_.array[0];
        // cal total
        uchar t = 0x00;
        for (uint i = 0; i < 6;++i){
            t += tor_arr[i];
        }
        tor_arr[6] = t;
    }
    return tor_arr;
}

void MotorDriver::ctlMotorSpd(double spd)
{
    if (isInit){
        emit sendErrText(QString("driver not init"));
    }
    QByteArray spd_str = calSpdData(QString::number(spd));
    if(serial_port_.write(spd_str) != spd_str.size()){
        emit sendErrText(QString("driver control error"));
    }

}

void MotorDriver::ctlMotorTor(double tor)
{
    if (isInit){
        emit sendErrText(QString("driver not init"));
    }
    QByteArray tor_str = calTorData(QString::number(tor));
    if(serial_port_.write(tor_str) != tor_str.size()){
        emit sendErrText(QString("driver control error"));
    }
}

void MotorDriver::getMotorData()
{

}
