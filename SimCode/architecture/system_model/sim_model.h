#ifndef _SimModel_HH_
#define _SimModel_HH_

#include <vector>
#include <stdint.h>
#include "architecture/system_model/sys_model_thread.h"
#include "architecture/messaging/system_messaging.h"
#include "architecture/system_model/message_logger.h"

/*! \addtogroup SimArchGroup
 *  This architecture group contains the source used to drive/schedule/interface
 *  with the simulation.
 * @{
 */

//! Structure that contains the information needed to call a thread
typedef struct {
    uint64_t NextThreadStart;  /*!< Time to call thread next*/
    uint64_t ThreadUpdatePeriod;   /*!< Period of update for thread*/
    SysModelThread *ThreadPtr;  /*!< Handle to the thread that needs to be called*/
}ModelScheduleEntry;

typedef enum varAccessType {
    messageBuffer = 0,
    logBuffer = 1
}VarAccessType;

//! The top-level container for an entire simulation
class SimModel
{
public:
    SimModel(); //!< The SimModel constructor
    ~SimModel();//!< SimModel destructor
    void AddNewThread(SysModelThread *NewThread); //!< Method to add new thread to sim
    void InitThreads();  //!< Method to initialize all added threads
    void StepUntilTime(uint64_t SimStopTime); //!< Step simulation until stop time uint64_t reached
    void SingleStepNextThread(); //!< Step only the next thread in the simulation
    void ScheduleThread(ModelScheduleEntry *ThreadCall); //!< Schedule the ThreadCall object passed in
    void PrintSimulatedMessageData(); //!< Print out all messages that have been created
    uint64_t GetWriteData(std::string MessageName, uint64_t MaxSize,
                          void *MessageData, VarAccessType logType = messageBuffer,
                          uint64_t LatestOffset=0); //!< Grab a particular MessageName with MaxSize limited
    void ResetSimulation(); //!< Reset simulation back to zero
    void WriteMessageData(std::string MessageName, uint64_t MessageSize,
                          uint64_t ClockTime, void *MessageData); //!< Write in a single message
    void CreateNewMessage(std::string MessageName, uint64_t MessageSize,
                          uint64_t NumBuffers=2); //!< Create a new message for use
    void logThisMessage(std::string messageName, uint64_t messagePeriod=0);
    
public:
    std::vector<ModelScheduleEntry> ThreadModels; //!< -- Array that has pointers to all GNC laws
    std::string SimulationName;                      //!< -- Identifier for thread
    uint64_t CurrentNanos;                        //!< ns Current clock time
    uint64_t NextThreadTime;                      //!< ns time for the next thread
    messageLogger messageLogs;                       //!< -- Message log data
private:
    BlankStorage MessageBucket;                      //!< -- Messaging data
};

/*! @} */
#endif /* _SimModel_H_ */