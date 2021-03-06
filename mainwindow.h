/**********************************************************************
 *  mainwindow.h
 **********************************************************************
 * Copyright (C) 2017 MX Authors
 *
 * Authors: Adrian, Dolphin Oracle
 *          MX Linux <http://mxlinux.org>
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QProgressBar>
#include <QTemporaryDir>
#include <QTimer>

#include <dialog.h>
#include <version.h>
#include <cmd.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString selectPartiton(const QStringList &list);
    QStringList getLinuxPartitions();

    void addGrubArg(const QString &key, const QString &item);
    void addGrubLine(const QString &item);
    void createChrootEnv(QString root);
    void disableGrubLine(const QString &item);
    void enableGrubLine(const QString &item);
    void loadPlymouthThemes();
    void readDefaultGrub();
    void readGrubCfg();
    void readKernelOpts();
    void remGrubArg(const QString &key, const QString &item);
    void sendMouseEvents();
    void setup();
    void writeDefaultGrub() const;

    bool checkInstalled(const QString &package);
    bool checkInstalled(const QStringList &packages);
    bool inVirtualMachine();
    bool installSplash();
    bool replaceGrubArg(const QString &key, const QString &item);

    int findMenuEntryById(const QString &id) const;


public slots:

private slots:
    void cleanup();
    void cmdDone();
    void cmdStart();
    void procTime();
    void setConnections();

    void on_btn_bg_file_clicked();
    void on_btn_theme_file_clicked();
    void on_buttonAbout_clicked();
    void on_buttonApply_clicked();
    void on_buttonHelp_clicked();
    void on_buttonLog_clicked();
    void on_button_preview_clicked();
    void on_cb_bootsplash_clicked(bool checked);
    void on_cb_bootsplash_toggled(bool checked);
    void on_cb_enable_flatmenus_clicked(bool checked);
    void on_cb_grub_theme_toggled(bool checked);
    void on_cb_save_default_clicked();
    void on_combo_menu_entry_currentIndexChanged(int index);
    void on_combo_theme_activated(int);
    void on_combo_theme_currentIndexChanged(const QString &arg1);
    void on_lineEdit_kernel_textEdited();
    void on_rb_detailed_msg_toggled(bool checked);
    void on_rb_limited_msg_toggled(bool checked);
    void on_rb_very_detailed_msg_toggled(bool checked);
    void on_spinBoxTimeout_valueChanged(int val);


protected:
    void keyPressEvent(QKeyEvent* event);
    QProgressBar *bar;

private:
    Ui::MainWindow *ui;
    Cmd cmd;
    QTimer timer;

    bool grub_installed;
    bool just_installed;
    bool kernel_options_changed;
    bool messages_changed;
    bool options_changed;
    bool splash_changed;

    QString chroot;
    QString kernel_options;
    QString user;
    QStringList default_grub;
    QStringList grub_cfg;
    QTemporaryDir tmpdir;
};


#endif

