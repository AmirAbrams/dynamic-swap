// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2009-2018 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_QT_TRANSACTIONTABLEMODEL_H
#define DYNAMIC_QT_TRANSACTIONTABLEMODEL_H

#include "dynamicunits.h"

#include <QAbstractTableModel>
#include <QStringList>

class PlatformStyle;
class TransactionRecord;
class TransactionTablePriv;
class WalletModel;

class CWallet;

/** UI model for the transaction table of a wallet.
 */
class TransactionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TransactionTableModel(const PlatformStyle* platformStyle, CWallet* wallet, WalletModel* parent = 0);
    ~TransactionTableModel();

    enum ColumnIndex {
        Status = 0,
        Watchonly = 1,
        InstantSend = 2,
        Date = 3,
        Type = 4,
        ToAddress = 5,
        Amount = 6
    };

    /** Roles to get specific information from a transaction row.
        These are independent of column.
    */
    enum RoleIndex {
        /** Type of transaction */
        TypeRole = Qt::UserRole,
        /** Date and time this transaction was created */
        DateRole,
        /** Watch-only boolean */
        WatchonlyRole,
        /** Watch-only icon */
        WatchonlyDecorationRole,
        /** InstantSend boolean */
        InstantSendRole,
        /** InstantSend icon */
        InstantSendDecorationRole,
        /** Long description (HTML format) */
        LongDescriptionRole,
        /** Address of transaction */
        AddressRole,
        /** Label of address related to transaction */
        LabelRole,
        /** Net amount of transaction */
        AmountRole,
        /** Unique identifier */
        TxIDRole,
        /** Transaction hash */
        TxHashRole,
        /** Transaction data, hex-encoded */
        TxHexRole,
        /** Whole transaction as plain text */
        TxPlainTextRole,
        /** Is transaction confirmed? */
        ConfirmedRole,
        /** Formatted amount, without brackets when unconfirmed */
        FormattedAmountRole,
        /** Transaction status (TransactionRecord::Status) */
        StatusRole,
        /** Unprocessed icon */
        RawDecorationRole,
    };

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    bool processingQueuedTransactions() { return fProcessingQueuedTransactions; }

private:
    CWallet* wallet;
    WalletModel* walletModel;
    QStringList columns;
    TransactionTablePriv* priv;
    bool fProcessingQueuedTransactions;
    const PlatformStyle* platformStyle;

    void subscribeToCoreSignals();
    void unsubscribeFromCoreSignals();

    QString lookupAddress(const std::string& address, bool tooltip) const;
    QVariant addressColor(const TransactionRecord* wtx) const;
    QString formatTxStatus(const TransactionRecord* wtx) const;
    QString formatTxDate(const TransactionRecord* wtx) const;
    QString formatTxType(const TransactionRecord* wtx) const;
    QString formatTxToAddress(const TransactionRecord* wtx, bool tooltip) const;
    QString formatTxAmount(const TransactionRecord* wtx, bool showUnconfirmed = true, DynamicUnits::SeparatorStyle separators = DynamicUnits::separatorStandard) const;
    QString formatTooltip(const TransactionRecord* rec) const;
    QVariant txStatusDecoration(const TransactionRecord* wtx) const;
    QVariant txWatchonlyDecoration(const TransactionRecord* wtx) const;
    QVariant txInstantSendDecoration(const TransactionRecord *wtx) const;
    QVariant txAddressDecoration(const TransactionRecord* wtx) const;

public Q_SLOTS:
    /* New transaction, or transaction changed status */
    void updateTransaction(const QString& hash, int status, bool showTransaction);
    void updateConfirmations();
    void updateDisplayUnit();
    /** Updates the column title to "Amount (DisplayUnit)" and emits headerDataChanged() signal for table headers to react. */
    void updateAmountColumnTitle();
    /* Needed to update fProcessingQueuedTransactions through a QueuedConnection */
    void setProcessingQueuedTransactions(bool value) { fProcessingQueuedTransactions = value; }

    friend class TransactionTablePriv;
};

#endif // DYNAMIC_QT_TRANSACTIONTABLEMODEL_H
