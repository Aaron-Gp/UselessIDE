#ifndef LANG_H
#define LANG_H

#include <QObject>
#include "../stdafx.h"

class Lang : public QObject
{
    Q_OBJECT
    Q_PROPERTY_AUTO(QString,home);
    Q_PROPERTY_AUTO(QString,basic_input);
    Q_PROPERTY_AUTO(QString,form);
    Q_PROPERTY_AUTO(QString,surface);
    Q_PROPERTY_AUTO(QString,popus);
    Q_PROPERTY_AUTO(QString,navigation);
    Q_PROPERTY_AUTO(QString,theming);
    Q_PROPERTY_AUTO(QString,media);
    Q_PROPERTY_AUTO(QString,dark_mode);
    Q_PROPERTY_AUTO(QString,sys_dark_mode);
    Q_PROPERTY_AUTO(QString,search);
    Q_PROPERTY_AUTO(QString,about);
    Q_PROPERTY_AUTO(QString,settings);
    Q_PROPERTY_AUTO(QString,navigation_view_display_mode);
    Q_PROPERTY_AUTO(QString,locale);
    Q_PROPERTY_AUTO(QString,other);

    Q_PROPERTY_AUTO(QString,file);
    Q_PROPERTY_AUTO(QString,new_);
    Q_PROPERTY_AUTO(QString,open);
    Q_PROPERTY_AUTO(QString,save);
    Q_PROPERTY_AUTO(QString,save_as);
    Q_PROPERTY_AUTO(QString,close);
    Q_PROPERTY_AUTO(QString,exit);
    Q_PROPERTY_AUTO(QString,edit);
    Q_PROPERTY_AUTO(QString,undo);
    Q_PROPERTY_AUTO(QString,redo);
    Q_PROPERTY_AUTO(QString,cut);
    Q_PROPERTY_AUTO(QString,copy);
    Q_PROPERTY_AUTO(QString,paste);
    Q_PROPERTY_AUTO(QString,indent);
    Q_PROPERTY_AUTO(QString,unindent);
    Q_PROPERTY_AUTO(QString,toggle_comment);
    Q_PROPERTY_AUTO(QString,search_);
    Q_PROPERTY_AUTO(QString,find);
    Q_PROPERTY_AUTO(QString,replace);
    Q_PROPERTY_AUTO(QString,project);
    Q_PROPERTY_AUTO(QString,new_project_file);
    Q_PROPERTY_AUTO(QString,new_class);
    Q_PROPERTY_AUTO(QString,new_header);
    Q_PROPERTY_AUTO(QString,add_to_project);
    Q_PROPERTY_AUTO(QString,remove_from_project);
    Q_PROPERTY_AUTO(QString,execute);
    Q_PROPERTY_AUTO(QString,compile);
    Q_PROPERTY_AUTO(QString,run);
    Q_PROPERTY_AUTO(QString,tools);
    Q_PROPERTY_AUTO(QString,options);
    Q_PROPERTY_AUTO(QString,help);
    Q_PROPERTY_AUTO(QString,about_);

    Q_PROPERTY_AUTO(QString,settings_);
    Q_PROPERTY_AUTO(QString,theme);
    Q_PROPERTY_AUTO(QString,system);
    Q_PROPERTY_AUTO(QString,light);
    Q_PROPERTY_AUTO(QString,dark);

    Q_PROPERTY_AUTO(QString,filename);
    Q_PROPERTY_AUTO(QString,line);
    Q_PROPERTY_AUTO(QString,col);
    Q_PROPERTY_AUTO(QString,description);
    Q_PROPERTY_AUTO(QString,lines);
    Q_PROPERTY_AUTO(QString,reformat);
    Q_PROPERTY_AUTO(QString,code);
    Q_PROPERTY_AUTO(QString,dir);
public:
    explicit Lang(QObject *parent = nullptr);

};

#endif // LANG_H
