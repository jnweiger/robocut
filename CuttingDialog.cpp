#include "CuttingDialog.h"
#include "ui_CuttingDialog.h"

#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>

#include <QDebug>

CuttingDialog::CuttingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CuttingDialog)
{
	ui->setupUi(this);
	thread = NULL;
}

CuttingDialog::~CuttingDialog()
{
	delete ui;
	if (thread)
	{
		thread->exit();
		thread->wait(1000);
	}
}

void CuttingDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}


void CuttingDialog::startCut(QList<QPolygonF> cuts, int media, int speed, int pressure, bool trackenhancing)
{
	if (thread)
	{
		qDebug() << "Internal error: startCut() called twice.";
		return;
	}
	thread = new CuttingThread(this);

	connect(thread, SIGNAL(success()), SLOT(onSuccess()));
	connect(thread, SIGNAL(error(QString)), SLOT(onError(QString)));

	thread->setParams(cuts, media, speed, pressure, trackenhancing);

	thread->start();
}

void CuttingDialog::onSuccess()
{
	ui->status->setText("Finished");
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Close");
	thread->exit();
	thread->wait(1000);
	delete thread;
	thread = NULL;
}

void CuttingDialog::onError(QString message)
{
	ui->status->setText("Error: " + message);
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Close");
	thread->exit();
	thread->wait(1000);
	delete thread;
	thread = NULL;
}

void CuttingDialog::closeEvent(QCloseEvent* e)
{
	if (thread)
	{
		if (QMessageBox::question(this, "Kill cutting process?", "Are you sure you wish to stop the cutting process? "
								  "This may result in bad stuff. It might be better just to turn the plotter off and on again.",
								  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
		{
			e->ignore();
			return;
		}

		thread->terminate();
		thread->wait(5000);
		delete thread;
		thread = NULL;
	}
	return QDialog::closeEvent(e);
}