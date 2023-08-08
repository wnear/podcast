#include "podcastchannel.h"
#include "jsonop.h"
#include "utils.h"
#include "downloadmanager.h"
#include "global.h"
#include "log.h"
#include "rssparser.h"

#include "sqlmanager.h"

QString PodcastChannel::datapath() const {
    return util::ensureDirExist(util::datapath(), this->m_feedTitle);
}

QString PodcastChannel::xmlfile() const {
    return QDir(this->datapath()).filePath(this->xmlfmt);
}

QString PodcastChannel::jsonfile() const {
    return QDir(this->datapath()).filePath(this->jsonfmt);
}

QString PodcastChannel::coverfile() const {
    return QDir(location).absoluteFilePath(coverfmt);
}

PodcastChannel::PodcastChannel(const QString &title, const QString &url, QObject *parent)
    : QObject(parent) {
    m_feedTitle = title;
    m_feedUrl = url;
    location = Data::podcastChannelDataPath(m_feedTitle);
}

// TODO: load should happen only on pod init and after pod update.
// update should diff with load, add only new episodes => more fluent ui.
bool PodcastChannel::load() {
    this->m_episodes.clear();
    SQLManager::instance()->loadEpisodes(this);
    return true;

    bool ret;
    QFile xml_file(xmlfile());
    QFile json_file(jsonfile());

    // if both data is not existed, download.
    if (json_file.exists() == false && xml_file.exists() == false) {
        this->updatexml();
        return false;
    }

    if (xml_file.exists()) {
        // if json not existed, or not update.
        if (json_file.exists() == false || util::file_is_newer(xml_file, xml_file)) {
            ret = this->parserxml();
            if (ret == false) return ret;
        }
    }

    // json should exist by now.
    ret = load_json();
    return ret;
}

bool PodcastChannel::load_json() { return jsonload(this, jsonfile()); }

bool PodcastChannel::updatexml() {
    auto &pod = *this;
    if (pod.job_id != -1) {
        auto status = DownloadManager::instance()->getJobStatus(pod.job_id);
        if (status == TASK_DOWNLOADING) {
            binfo("some one want's to try download  a pod twice?");
            return false;
        } else {
            binfo("retry a failed xml download({})", pod.url.toStdString());
            binfo("last time the job ret is {}.", status);
            qDebug() << status;
        }
    }
    // auto res = xmlfile();
    pod.job_id = DownloadManager::instance()->addjob(pod.m_feedUrl, xmlfile());
    connect(
        DownloadManager::instance(), &DownloadManager::stateChanged, this,
        [&pod, this](int id, auto st) {
            if (id == pod.job_id && st == TASK_COMPLETE) {
                this->parserxml();
            }
        },
        Qt::SingleShotConnection);
    return true;
}

bool PodcastChannel::parserxml() {
    auto &pod = *this;
    QString xml = xmlfile();
    QFile xml_file(xml);
    auto parser = RssParser(&xml_file, &pod);
    bool ret = parser.parse();
    if (!pod.cover_url.isEmpty())
        if (!QFile(pod.coverfile()).exists()) {
            binfo("download cover");
            DownloadManager::instance()->addjob(pod.cover_url, pod.coverfile());
        }
    if (ret) {
        this->save();
    }
    return ret;
}

bool PodcastChannel::save() { return jsonsave(this, jsonfile()); }

// TODO: deal with memory is tricky here.
//
// FIXME: should delete memory of old episodes.
void PodcastChannel::addEpisodes(QList<EpisodeData *> &eps) {
    std::reverse(eps.begin(), eps.end());
    bool st{true};
    st = std::is_sorted(eps.begin(), eps.end(),
                   [](auto &&lhs, auto &&rhs) { return lhs->updatetime < rhs->updatetime; });
    assert(st == true);
    auto it_new = std::lower_bound(
        eps.begin(), eps.end(), this->lastEpisodeUpdate,
        [](EpisodeData *cur, QDateTime val) { return cur->updatetime > val; });
    if (it_new == eps.end()) return;

    bool dev = false;
    if (dev) {
        for (; it_new < eps.end(); it_new++) {
            // SQLManager::instance()->findEpisode(1,1);
            QDateTime dt_cur = (*it_new)->updatetime;
            auto ep = std::find_if(m_episodes.begin(), m_episodes.end(),
            [dt_cur](EpisodeData *cur){
                return cur->updatetime == dt_cur;
            });

            (*it_new)->id = (*ep)->id;

            SQLManager::instance()->reinitEpisode(*it_new);
        }
    }

    for (; it_new < eps.end(); it_new++) {
        this->addEpisode(*it_new);
    }
    lastEpisodeUpdate = eps.back()->updatetime;
}

void PodcastChannel::addEpisode(EpisodeData *ep) {
    SQLManager::instance()->addEpisode(this->channelID, ep);
    m_episodes.push_front(ep);
}
