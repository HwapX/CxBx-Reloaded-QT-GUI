#include "xbetablemodel.h"

void freeLogo(void *info)
{
    delete (uint8_t*)info;
}

QString regionToString(const uint32_t dwRegion)
{
    QString region;
    if(dwRegion & (XBEIMAGE_GAME_REGION_NA | XBEIMAGE_GAME_REGION_JAPAN | XBEIMAGE_GAME_REGION_RESTOFWORLD))
        region = "ALL";
    else if(dwRegion & XBEIMAGE_GAME_REGION_NA)
        region = "NA";
    else if(dwRegion & XBEIMAGE_GAME_REGION_JAPAN)
        region = "JP";
    else if(dwRegion & XBEIMAGE_GAME_REGION_RESTOFWORLD)
        region = "RW";
    else
        region = "??";

    if(dwRegion & XBEIMAGE_GAME_REGION_MANUFACTURING)
        region += "-M";

    return QString("0x%1 (%2)").arg(QString::number(dwRegion, 16).toUpper(), 8, '0').arg(region);
}

XbeTableModel::XbeTableModel(QString directory)
{
    scanDirectory(directory);
}

XbeTableModel::~XbeTableModel()
{
    while(!this->xbeList.empty()) {
        delete this->xbeList.back();
        this->xbeList.pop_back();
    }
}

int XbeTableModel::rowCount(const QModelIndex &parent) const
{
    return this->xbeList.count();
}

int XbeTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant XbeTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= this->xbeList.count())
        return QVariant();

    Xbe *xbe = this->xbeList.at(index.row());

    switch(role) {
    case Qt::DisplayRole:
        switch(index.column()) {
        case 0: return this->logoCache[xbe];
        case 1: return QString(xbe->m_szAsciiTitle);
        case 2: return regionToString(xbe->m_Certificate.dwGameRegion);
        case 3: return QString::asprintf("%d.%d.%d", xbe->m_LibraryVersion->wMajorVersion, xbe->m_LibraryVersion->wMinorVersion, xbe->m_LibraryVersion->wBuildVersion);
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == 0)
            return this->logoCache[xbe];
        break;
    case Qt::TextAlignmentRole:
        switch(index.column()) {
        case 2:
        case 3:
            return Qt::AlignCenter;
        }
        break;
    }

    return QVariant();
}

void XbeTableModel::scanDirectory(QString directory)
{
    QDirIterator it(directory, QStringList() << "*.xbe", QDir::Files,
                    QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    Xbe *xbe = NULL;

    while(it.hasNext()) {
        QString nextFile = it.next();

        xbe = new Xbe(nextFile.toStdString().data());

        uint8_t *a = new uint8_t[100*17];
        xbe->ExportLogoBitmap(a);

        QImage logo(a, 100, 17, QImage::Format_Grayscale8, freeLogo, (void*)a);

        this->xbeList.append(xbe);
        this->logoCache[xbe] = QPixmap::fromImage(logo);
    }
}

QVariant XbeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return section + 1;

    switch(section) {
    case 0: return "Logo";
    case 1: return "Title";
    case 2: return "Region";
    case 3: return "SDK Version";
    default: return QVariant();
    }
}

Xbe *XbeTableModel::getXbe(int index)
{
    return this->xbeList.at(index);
}
