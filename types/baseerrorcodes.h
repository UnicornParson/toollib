#ifndef BASEERRORCODES_H
#define BASEERRORCODES_H

#include <QObject>
namespace Tools
{

  class BaseErrorCodes : public QObject
  {
    Q_OBJECT
  public:
    enum e
    {
      NoError = 0, // anything is ok
      Arguments, // invalid arguments are provided to function
      ObjectState, // object in invalid state (not initialized?)
      Configuration, // incorrect configuraton that required for operation
      Network, // network error
      RemoteError, //network is ok but back-end returns error code
      Unknown, // anything else
      InProgress, // operation in progress. need to wait
      Postponed, // operation is postponed
      Done, // operation is completed
      AsyncDone, // operation requested asynchronously
      InvalidAddress, //check adress param
      FileExists, // file already exists
      ServerNotRespond, // server request timeout
      TargetNotFound, // 404
      FileNotFound, // file not found or cannot be opened
      FSError, // error in fs operation
      IOError, // base io error
      CompilationError, // runtime compilation error
      ParsingError, // data parsing failed
      NoDataError, // data source (file/buffer) is empty
      NotImplemented, // feature is not implemented and cannot be used
      EXTENDED_REASON_CODE = 128 // end of universal reasons list
    };
    Q_ENUM(e)

    static QString toString(BaseErrorCodes::e val);
    static bool isError(BaseErrorCodes::e val);
    static bool isUniversalReason(int val);

  private:
    explicit BaseErrorCodes(QObject *parent = nullptr);

  }; // class BaseErrorCodes

  typedef BaseErrorCodes::e eBaseErrorCodes;

} // namespace Tools
#endif // BASEERRORCODES_H
