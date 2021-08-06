#ifndef SEARCHRESULTVIEW_H
#define SEARCHRESULTVIEW_H

#include <QTreeView>
#include <QMap>
#include <QStyledItemDelegate>
#include "../qsynedit/SearchBase.h"
#include "utils.h"

#define MAX_SEARCH_RESULTS 20
struct SearchResultTreeItem;
using PSearchResultTreeItem = std::shared_ptr<SearchResultTreeItem>;
using SearchResultTreeItemList = QList<PSearchResultTreeItem>;
using PSearchResultTreeItemList = std::shared_ptr<SearchResultTreeItemList>;

struct SearchResultTreeItem {
    QString filename;
    int line;
    int start;
    int len;
    QString text;
    SearchResultTreeItem* parent;
    SearchResultTreeItemList results;
};



struct SearchResults{
    SynSearchOptions options;
    QString keyword;
    SearchFileScope scope;
    QList<PSearchResultTreeItem> results;
};

using PSearchResults = std::shared_ptr<SearchResults>;

class SearchResultModel : public QObject {
    Q_OBJECT
public:
    explicit SearchResultModel(QObject* parent=nullptr);
    PSearchResults addSearchResults(const QString& keyword,SynSearchOptions options,
                                    SearchFileScope scope);
    PSearchResults results(int index);
    void notifySearchResultsUpdated();
    int currentIndex() const;
    int resultsCount() const;
    PSearchResults currentResults();
    void setCurrentIndex(int index);
    void clear();
signals:
    void modelChanged();
    void currentChanged(int index);
private:
    QList<PSearchResults> mSearchResults;
    int mCurrentIndex;

};

class SearchResultListModel: public QAbstractListModel {
Q_OBJECT
    // QAbstractItemModel interface
public:
    explicit SearchResultListModel(SearchResultModel* model,QObject* parent=nullptr);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
public slots:
    void onResultModelChanged();
private:
    SearchResultModel *mSearchResultModel;
};

using PSearchResultListModel = std::shared_ptr<SearchResultListModel>;

class SearchResultTreeModel : public QAbstractItemModel {
Q_OBJECT
    // QAbstractItemModel interface
public:
    explicit SearchResultTreeModel(SearchResultModel* model,QObject* parent=nullptr);
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    SearchResultModel *searchResultModel() const;
public slots:
    void onResultModelChanged();
private:
    SearchResultModel *mSearchResultModel;
};

using PSearchResultTreeModel = std::shared_ptr<SearchResultTreeModel>;

class SearchResultTreeViewDelegate: public QStyledItemDelegate{
Q_OBJECT
    // QAbstractItemDelegate interface
public:
    explicit SearchResultTreeViewDelegate(PSearchResultTreeModel model,
                                          QObject* parent=nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    PSearchResultTreeModel mModel;
};

using PSearchResultTreeViewDelegate = std::shared_ptr<SearchResultTreeViewDelegate>;

#endif // SEARCHRESULTVIEW_H