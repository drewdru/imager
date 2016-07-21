#include "PluginInfo.h"

PluginInfo::PluginInfo()
{
    pluginPath = "";
    arguments = "";
    authorInfo = "";
    version = 0;
}

PluginInfo::PluginInfo(QString pluginPath, QString arguments, QString authorInfo, float version)
{
    this->pluginPath = pluginPath;
    this->arguments = arguments;
    this->authorInfo = authorInfo;
    this->version = version;
}

QString PluginInfo::getPluginPath()
{
    return pluginPath;
}

QString PluginInfo::getArguments()
{
    return arguments;
}

QString PluginInfo::getAuthorInfo()
{
    return authorInfo;
}

float PluginInfo::getVersion()
{
    return version;
}
