QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ../nodnok/chisla.h
SOURCES +=  tst_test.cpp ../nodnok/chisla.cpp
