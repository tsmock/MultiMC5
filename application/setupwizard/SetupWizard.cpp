#include "SetupWizard.h"
#include "translations/TranslationsModel.h"
#include <MultiMC.h>
#include <FileSystem.h>
#include <ganalytics.h>

enum Page
{
	Language,
	Java,
	Analytics,
	Themes,
	Accounts
};
#include "ui_SetupWizard.h"


SetupWizard::SetupWizard(QWidget *parent) : QWizard(parent), ui(new Ui::SetupWizard)
{
	ui->setupUi(this);
	// FIXME: invert - instead of removing already present pages, create them here on demand.
	if (languageIsRequired())
	{
		auto translations = MMC->translations();
		auto index = translations->selectedIndex();
		ui->languageView->setModel(translations.get());
		ui->languageView->setCurrentIndex(index);
		connect(ui->languageView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SetupWizard::languageRowChanged);
	}
	else
	{
		removePage(Page::Language);
	}
	if(javaIsRequired())
	{
		// set up java selection
	}
	else
	{
		removePage(Page::Java);
	}
	if(analyticsIsRequired())
	{
		// set up analytics???
	}
	else
	{
		removePage(Page::Analytics);
	}
	removePage(Page::Themes);
	removePage(Page::Accounts);
}

void SetupWizard::retranslate()
{
	ui->retranslateUi(this);
	setButtonText(QWizard::NextButton, tr("Next >"));
	setButtonText(QWizard::BackButton, tr("< Back"));
	setButtonText(QWizard::FinishButton, tr("Finish"));
}

void SetupWizard::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		retranslate();
	}
	QWizard::changeEvent(event);
}

void SetupWizard::languageRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	if (current == previous)
		return;

	auto translations = MMC->translations();
	QString key = translations->data(current, Qt::UserRole).toString();
	translations->selectLanguage(key);
	translations->updateLanguage(key);
}

SetupWizard::~SetupWizard()
{
}

bool SetupWizard::languageIsRequired()
{
	auto settings = MMC->settings();
	if (settings->get("Language").toString().isEmpty())
		return true;
	return false;
}

bool SetupWizard::javaIsRequired()
{
	QString currentHostName = QHostInfo::localHostName();
	QString oldHostName = MMC->settings()->get("LastHostname").toString();
	if (currentHostName != oldHostName)
	{
		MMC->settings()->set("LastHostname", currentHostName);
		return true;
	}
	QString currentJavaPath = MMC->settings()->get("JavaPath").toString();
	QString actualPath = FS::ResolveExecutable(currentJavaPath);
	if (actualPath.isNull())
	{
		return true;
	}
	return false;
}

bool SetupWizard::analyticsIsRequired()
{
	auto settings = MMC->settings();
	auto analytics = MMC->analytics();
	if(settings->get("AnalyticsSeen").toInt() < analytics->version())
	{
		return true;
	}
	return false;
}

bool SetupWizard::isRequired()
{
	if (languageIsRequired())
		return true;
	if (javaIsRequired())
		return true;
	if (analyticsIsRequired())
		return true;
	return false;
}

/*
void MainWindow::checkSetDefaultJava()
{
	qDebug() << "Java path needs resetting, showing Java selection dialog...";

	JavaInstallPtr java;

	VersionSelectDialog vselect(MMC->javalist().get(), tr("Select a Java version"), this, false);
	vselect.setResizeOn(2);
	vselect.exec();

	if (vselect.selectedVersion())
		java = std::dynamic_pointer_cast<JavaInstall>(vselect.selectedVersion());
	else
	{
		CustomMessageBox::selectable(this, tr("Invalid version selected"), tr("You didn't select a valid Java version, so MultiMC will "
																				"select the default. "
																				"You can change this in the settings dialog."),
										QMessageBox::Warning)
			->show();

		JavaUtils ju;
		java = ju.GetDefaultJava();
	}
	if (java)
	{
		MMC->settings()->set("JavaPath", java->path);
	}
	else
		MMC->settings()->set("JavaPath", QString("java"));
}
*/