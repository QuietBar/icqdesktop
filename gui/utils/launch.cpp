#include "stdafx.h"
#include "launch.h"
#include "application.h"
#include "../types/chat.h"
#include "../types/contact.h"
#include "../types/images.h"
#include "../types/link_metadata.h"
#include "../types/message.h"
#include "../types/typing.h"
#include "../cache/stickers/stickers.h"
#include "../main_window/contact_list/ContactListModel.h"
#include "../main_window/history_control/MessagesModel.h"
#include "../main_window/tray/RecentMessagesAlert.h"

#ifdef ICQ_QT_STATIC
    #ifdef _WIN32
        Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
        Q_IMPORT_PLUGIN(QICOPlugin);
    #endif //_WIN32

    #ifndef __linux__
        Q_IMPORT_PLUGIN(QTiffPlugin)
    #else
        Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
        Q_IMPORT_PLUGIN(QICOPlugin);
    #endif //__linux__

    #ifdef __APPLE__
        Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
        Q_IMPORT_PLUGIN(QICOPlugin);
    #endif
#endif

static const std::string urlCommand = "-urlcommand";

launch::CommandLineParser::CommandLineParser(int _argc, char* _argv[])
{
    isUlrCommand_ = false;

    if (_argc <= 0)
    {
        return;
    }

    executable_ = _argv[0];

    for (int i = 1; i < _argc; ++i)
    {
        if (_argv[i] == urlCommand)
        {
            isUlrCommand_ = true;

            ++i;

            if (i >= _argc)
                break;

            urlCommand_ = QString::fromUtf8(_argv[i]);
        }
    }
}

launch::CommandLineParser::~CommandLineParser()
{

}

bool launch::CommandLineParser::isUrlCommand() const
{
    return isUlrCommand_;
}

const QString& launch::CommandLineParser::getUrlCommand() const
{
    return urlCommand_;
}

const QString& launch::CommandLineParser::getExecutable() const
{
    return executable_;
}

int launch::main(int _argc, char* _argv[])
{
    static bool isLaunched = false;
    if (isLaunched)
        return 0;
    isLaunched = true;

    Utils::Application app(_argc, _argv);

    CommandLineParser cmd_parser(_argc, _argv);

    if (!app.isMainInstance())
    {
        app.switchInstance(cmd_parser);

        return 0;
    }

    if (app.updating())
    {
        return 0;
    }

    if (app.init())
    {
        qRegisterMetaType<Data::ImageListPtr>("Data::ImageListPtr");
        qRegisterMetaType<std::shared_ptr<Data::ContactList>>("std::shared_ptr<Data::ContactList>");
        qRegisterMetaType<std::shared_ptr<Data::Buddy>>("std::shared_ptr<Data::Buddy>");
        qRegisterMetaType<Data::MessageBuddies>("Data::MessageBuddies");
        qRegisterMetaType<std::shared_ptr<Data::ChatInfo>>("std::shared_ptr<Data::ChatInfo>");
        qRegisterMetaType<QVector<Data::ChatInfo>>("QVector<Data::ChatInfo>");
        qRegisterMetaType<QVector<Data::ChatMemberInfo>>("QVector<Data::ChatMemberInfo>");
        qRegisterMetaType<Data::DlgState>("Data::DlgState");
        qRegisterMetaType<Logic::scroll_mode_type>("Logic::scroll_mode_type");
        qRegisterMetaType<Logic::MessageKey>("Logic::MessageKey");
        qRegisterMetaType<Logic::UpdateChatSelection>("Logic::UpdateChatSelection");
        qRegisterMetaType<QVector<Logic::MessageKey>>("QVector<Logic::MessageKey>");
        qRegisterMetaType<QScroller::State>("QScroller::State");
        qRegisterMetaType<Ui::MessagesBuddiesOpt>("Ui::MessagesBuddiesOpt");
        qRegisterMetaType<QSystemTrayIcon::ActivationReason>("QSystemTrayIcon::ActivationReason");
        qRegisterMetaType<Logic::TypingFires>("Logic::TypingFires");
        qRegisterMetaType<int64_t>("int64_t");
        qRegisterMetaType<int32_t>("int32_t");
        qRegisterMetaType<uint64_t>("uint64_t");
        qRegisterMetaType<uint32_t>("uint32_t");
        qRegisterMetaType<Data::LinkMetadata>("Data::LinkMetadata");
        qRegisterMetaType<QSharedPointer<QMovie>>("QSharedPointer<QMovie>");
        qRegisterMetaType<Data::Quote>("Data::Quote");
        qRegisterMetaType<QVector<Data::Quote>>("QVector<Data::Quote>");
        qRegisterMetaType<QVector<Data::DlgState>>("QVector<Data::DlgState>");
        qRegisterMetaType<std::shared_ptr<Ui::Stickers::Set>>("std::shared_ptr<Ui::Stickers::Set>");
        qRegisterMetaType<Data::MessageBuddySptr>("Data::MessageBuddySptr");
        qRegisterMetaType<Ui::AlertType>("Ui::AlertType");
    }
    else
    {
        return 1;
    }


    //do not change context(1st argument), it's important
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Undo");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Redo");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Cu&t");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Copy");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Paste");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Delete");
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Select All");

#if defined(ICQ_FINAL_BUILD)
    qDebug() << ql1s("Final build is now running");
#elif defined(ICQ_DEVELOPMENT_BUILD)
    qDebug() << ql1s("Development build is now running");
#endif

    return app.exec();
}
