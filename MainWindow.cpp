#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include "commoncontrols.h"

#include <QFileIconProvider>
#include <QtWinExtras/QtWinExtras>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

QIcon iconFromExtensionExtraLarge(QString const &ext)
{
	QIcon icon;
	QString name = "*." + ext;
	SHFILEINFOW shinfo;
	if (SHGetFileInfoW((wchar_t const *)name.utf16(), 0, &shinfo, sizeof(shinfo), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES) != 0) {
		IImageList *pImageList;
		if (SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void **)&pImageList) == S_OK) {
			HICON hIcon;
			if (pImageList->GetIcon(shinfo.iIcon, ILD_TRANSPARENT, &hIcon) == S_OK) {
				QPixmap pm = QtWin::fromHICON(hIcon);
				if (!pm.isNull()) {
					icon = QIcon(pm);
				}
				DestroyIcon(hIcon);
			}
			pImageList->Release();
		}
	}
	return icon;
}

static QIcon iconFromExtension_(QString const &ext, UINT flag)
{
	QIcon icon;
	QString name = "*." + ext;
	SHFILEINFOW shinfo;
	if (SHGetFileInfoW((wchar_t const *)name.utf16(), 0, &shinfo, sizeof(shinfo), flag | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES) != 0) {
		QPixmap pm = QtWin::fromHICON(shinfo.hIcon);
		if (!pm.isNull()) {
			icon = QIcon(pm);
		}
	}
	return icon;
}

QIcon iconFromExtensionLarge(QString const &ext)
{
	return iconFromExtension_(ext, SHGFI_LARGEICON);
}

QIcon iconFromExtensionSmall(QString const &ext)
{
	return iconFromExtension_(ext, SHGFI_SMALLICON);
}

void MainWindow::on_pushButton_clicked()
{
	QIcon icon = iconFromExtensionLarge("html");
	if (icon.isNull()) {
		icon = QFileIconProvider().icon(QFileIconProvider::File);
	}
	ui->label->setPixmap(icon.pixmap(256, 256));
}
