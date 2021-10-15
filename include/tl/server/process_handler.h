/*************************************************************************
 > File Name: process_handler.h
 > Author: yafeng.zhao 
 > Mail: wakeup_7am@foxmail.com
 > Created Time: 五 10/15 01:45:46 2021
 ************************************************************************/

#pragma once
#ifndef _PROCESS_HANDLER_H
#define _PROCESS_HANDLER_H

#include <array>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
class PeocessHandler{
public:
    ProcessHandler(boost::asio::io_service &ios) : m_sock(ios){}
    
    ~ProcessHandler(){}

    void Read(){

    }
    
    void AsyncRead(){

    }

    void Write(){

    }

    void AsyncWrite(){

    }

    boost::asio::tcp::socket& GetSocket(){
        returm m_sock;
    }



};


#endif
