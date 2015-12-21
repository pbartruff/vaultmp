#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-pedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "RakNet/Source/RakPeerInterface.h"
#include "RakNet/Source/PacketizedTCP.h"
#include "RakNet/Source/MessageIdentifiers.h"
#include "RakNet/Source/FileListTransfer.h"
#include "RakNet/Source/FileListTransferCBInterface.h"
#include "RakNet/Source/IncrementalReadInterface.h"
#include "RakNet/Source/BitStream.h"
#include "RakNet/Source/RakString.h"
#include "RakNet/Source/RakSleep.h"
#include "RakNet/Source/GetTime.h"
#include "RakNet/Source/gettimeofday.h"
#include "RakNet/Source/NetworkIDObject.h"
#include "RakNet/Source/NetworkIDManager.h"

#pragma GCC diagnostic pop
