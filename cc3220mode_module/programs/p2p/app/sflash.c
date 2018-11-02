/*
 * sflash.c
 *
 *  Created on: Sep 4, 2018
 *      Author: Admin
 */

#include "sflash.h"
#include "userconfig.h"
#include "main_thread.h"


//62.5K is max file size
//#define MAX_FILE_SIZE        (63 * 1024)
#define MAX_FILE_SIZE        (100)
#define SL_MAX_FILE_SIZE     (2 * 1024)
//#define TEST_FILENAME        "/sys/test1.txt"
//#define TEST_FILENAME1        "file1.txt"
#define TEST_CERTIFICATE    "dummy-trusted-cert"
//char*           DeviceFileName1 = "MyFile.txt";
_const char originalContent[] = "This is the original content!";
_const char secondaryContent[] = "Overridden by the secondary content...";

#define USER_FILE_NAME          "configure.ini"


 _i32 st_ShowStorageInfo()
{
    _i32        RetVal = 0;
    _i32        size;
    _i32        used;
    _i32        avail;
    SlFsControlGetStorageInfoResponse_t storageInfo;

    UART_PRINT("\n\rGet Storage Info:\n\r");

    RetVal = sl_FsCtl(( SlFsCtl_e)SL_FS_CTL_GET_STORAGE_INFO,
                   0,
                   NULL ,
                   NULL ,
                   0,
                   (_u8 *)&storageInfo,
                   sizeof(SlFsControlGetStorageInfoResponse_t),
                   NULL );

    if(RetVal < 0)
    {
        UART_PRINT("sl_FsCtl error: %d\n\r");
    }

    size = (storageInfo.DeviceUsage.DeviceBlocksCapacity *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;
    UART_PRINT("Total space: %dK\n\r\n\r", size);

    UART_PRINT("Filestsyem      Size \tUsed \tAvail\t\n\r");

    size = ((storageInfo.DeviceUsage.NumOfAvailableBlocksForUserFiles +
            storageInfo.DeviceUsage.NumOfAllocatedBlocks) *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;

    used = (storageInfo.DeviceUsage.NumOfAllocatedBlocks *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;

    avail = (storageInfo.DeviceUsage.NumOfAvailableBlocksForUserFiles *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;

    UART_PRINT("%-15s %dK \t%dK \t%dK \t\n\r", "User", size, used, avail);

    size = (storageInfo.DeviceUsage.NumOfReservedBlocksForSystemfiles *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;

    UART_PRINT("%-15s %dK \n\r", "System", size);

    size = (storageInfo.DeviceUsage.NumOfReservedBlocks *
            storageInfo.DeviceUsage.DeviceBlockSize) / 1024;

    UART_PRINT("%-15s %dK \n\r", "Reserved", size);

    UART_PRINT("\n\r");
    UART_PRINT("\n\r");

    UART_PRINT("%-32s: %d \n\r", "Max number of files",
               storageInfo.FilesUsage.MaxFsFiles);

    UART_PRINT("%-32s: %d \n\r", "Max number of system files",
               storageInfo.FilesUsage.MaxFsFilesReservedForSysFiles);

    UART_PRINT("%-32s: %d \n\r", "Number of user files",
               storageInfo.FilesUsage.ActualNumOfUserFiles);

    UART_PRINT("%-32s: %d \n\r", "Number of system files",
               storageInfo.FilesUsage.ActualNumOfSysFiles);

    UART_PRINT("%-32s: %d \n\r", "Number of alert",
               storageInfo.FilesUsage.NumOfAlerts);

    UART_PRINT("%-32s: %d \n\r", "Number Alert threshold",
               storageInfo.FilesUsage.NumOfAlertsThreshold);

    UART_PRINT("%-32s: %d \n\r", "FAT write counter",
               storageInfo.FilesUsage.FATWriteCounter);

    UART_PRINT("%-32s: ", "Bundle state");

    if(storageInfo.FilesUsage.Bundlestate == SL_FS_BUNDLE_STATE_STOPPED)
    {
        UART_PRINT("%s \n\r", "Stopped");
    }
    else if(storageInfo.FilesUsage.Bundlestate == SL_FS_BUNDLE_STATE_STARTED)
    {
        UART_PRINT("%s \n\r", "Started");
    }
    else if(storageInfo.FilesUsage.Bundlestate ==
            SL_FS_BUNDLE_STATE_PENDING_COMMIT)
    {
        UART_PRINT("%s \n\r", "Commit pending");
    }

    UART_PRINT("\n\r");

    return RetVal;
}

#define MAX_FILE_ENTRIES 4

typedef struct
{
    SlFileAttributes_t attribute;
    char fileName[SL_FS_MAX_FILE_NAME_LENGTH];
} slGetfileList_t;

_i32 st_listFiles(_i32 numOfFiles, int bPrintDescription)
{
    int retVal = SL_ERROR_BSD_ENOMEM;
    _i32            index = -1;
    _i32 fileCount = 0;
    slGetfileList_t *buffer = malloc(MAX_FILE_ENTRIES * sizeof(slGetfileList_t));

    UART_PRINT("\n\rRead files list:\n\r");
    if(buffer)
    {
        while( numOfFiles > 0 )
        {
            _i32 i;
            _i32 numOfEntries = (numOfFiles < MAX_FILE_ENTRIES) ? numOfFiles : MAX_FILE_ENTRIES;

            // Get FS list
            retVal = sl_FsGetFileList(&index,
                                      numOfEntries,
                                      sizeof(slGetfileList_t),
                                      (_u8*)buffer,
                                      SL_FS_GET_FILE_ATTRIBUTES);
            if(retVal < 0)
            {
                UART_PRINT("sl_FsGetFileList error:  %d\n\r", retVal);
                break;
            }
            if(retVal == 0)
            {
                break;
            }

            // Print single column format
            for (i = 0; i < retVal; i++)
            {
                UART_PRINT("[%3d] ", ++fileCount);
                UART_PRINT("%-40s\t", buffer[i].fileName);
                UART_PRINT("%8d\t", buffer[i].attribute.FileMaxSize);
                UART_PRINT("0x%03x\t", buffer[i].attribute.Properties);
                UART_PRINT("\n\r");
            }
            numOfFiles -= retVal;
        }
        UART_PRINT("\n\r");

        if(bPrintDescription)
        {
            UART_PRINT(" File properties flags description:\n\r");
            UART_PRINT(" 0x001 - Open file commit\n\r");
            UART_PRINT(" 0x002 - Open bundle commit\n\r");
            UART_PRINT(" 0x004 - Pending file commit\n\r");
            UART_PRINT(" 0x008 - Pending bundle commit\n\r");
            UART_PRINT(" 0x010 - Secure file\n\r");
            UART_PRINT(" 0x020 - No file safe\n\r");
            UART_PRINT(" 0x040 - System file\n\r");
            UART_PRINT(" 0x080 - System with user access\n\r");
            UART_PRINT(" 0x100 - No valid copy\n\r");
            UART_PRINT(" 0x200 - Public write\n\r");
            UART_PRINT(" 0x400 - Public read\n\r");
            UART_PRINT("\n\r");
        }
        free (buffer);
    }

    return retVal;
}

#if 0
int InitFile1()
{

	unsigned long   MaxSize = 63 * 1024; //62.5K is max file size

	_i32            RetVal;
	unsigned long   Offset = 0;

	_u32 Token;
	SlFsFileInfo_t * FsFileInfo;
	unsigned char   InputBuffer[100];
	long            DeviceFileHandle;
	_u32            MasterToken;

	RetVal = sl_FsGetInfo(DeviceFileName1,0,&FsFileInfo);

	if(RetVal == 0)
	{
		UART_PRINT("write beacause file  exists: %d\n\r",RetVal);
		Offset = 0;
//	    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
//	    		(SL_FS_WRITE | SL_FS_CREATE_FAILSAFE  | SL_FS_CREATE_MAX_SIZE(MaxSize)),
//	    		&MasterToken);
	    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
	    		(SL_FS_WRITE
	    		 | SL_FS_CREATE_MAX_SIZE( MaxSize )),
	    		&MasterToken);
		RetVal = sl_FsWrite( DeviceFileHandle, Offset, (unsigned char *)"HelloWorld1", strlen("HelloWorld1"));

	    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);

	    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
	                                  SL_FS_READ,
	                                  &MasterToken);

	    Offset = 0;
	    RetVal = sl_FsRead( DeviceFileHandle, Offset, (unsigned char *)InputBuffer, strlen("HelloWorld1"));
	    UART_PRINT("InputBuffer1:%s\n\r",InputBuffer);
	    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);
	}

	else
	{
		UART_PRINT("not file  exists\n\r");
//		DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
//				(SL_FS_CREATE | SL_FS_CREATE_FAILSAFE  | SL_FS_CREATE_MAX_SIZE(MaxSize)),
//				&MasterToken);
		DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
				(SL_FS_CREATE| SL_FS_WRITE  |
				SL_FS_CREATE_MAX_SIZE( MaxSize )),
				&MasterToken);
		if(DeviceFileHandle < 0)
		{
			UART_PRINT("sl_FsOpen error112: %d\n\r", DeviceFileHandle );
		}
		Offset = 0;
	    RetVal = sl_FsWrite( DeviceFileHandle, Offset, (unsigned char *)"HelloWorld", strlen("HelloWorld"));

	    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);

	    DeviceFileHandle =  sl_FsOpen((unsigned char *)DeviceFileName1,
	                                  SL_FS_READ,
	                                  &MasterToken);

	    Offset = 0;
	    RetVal = sl_FsRead( DeviceFileHandle, Offset, (unsigned char *)InputBuffer, strlen("HelloWorld"));
	    UART_PRINT("InputBuffer:%s\n\r",InputBuffer);
	    RetVal = sl_FsClose(DeviceFileHandle, NULL, NULL , 0);
	}




	UART_PRINT("DeviceFileHandle: %u\n\r", DeviceFileHandle);
	UART_PRINT("DeviceFileHandle: %x\n\r", DeviceFileHandle);


	return RetVal;
}

#endif


int st_writeFile( _i32 fileHandle, _u32 length, _const char *buffer)
{
    int RetVal;
    _i32 offset = 0;

    while(offset < length)
    {
        // write data to open file
        RetVal = sl_FsWrite(fileHandle,
                         offset,
                         (_u8 *)buffer,
                         length);
        if (RetVal <= 0)
        {
            UART_PRINT("sl_FsWrite error:  %d\n\r" ,RetVal);
            return RetVal;
        }
        else
        {
            offset += RetVal;
            length -= RetVal;
        }

    }
    UART_PRINT("Wrote %d bytes...\n\r", offset);
    return offset;
}

int st_readFile( _i32 fileHandle, _u32 length)
{
    int offset = 0;
    int RetVal = 0;
    char buff_In[100] = {0};
    while(offset < length)
    {
        RetVal = sl_FsRead(fileHandle, offset, (_u8*)buff_In, length);

        if(RetVal == SL_ERROR_FS_OFFSET_OUT_OF_RANGE)
        {// EOF
            break;
        }
        if(RetVal < 0)
        {// Error
            UART_PRINT("sl_FsRead error: %d\n\r", RetVal);
            return RetVal;
        }

        offset += RetVal;
        length -= RetVal;
    }
    UART_PRINT("Read \"%s\" (%d bytes)...\n\r", buff_In, offset);

    return offset;
}

#if 0
int InitFile_Test()
{
	_i32            RetVal;
	_i32 fd = -1;

	fd = sl_FsOpen((unsigned char *)TEST_FILENAME1, (SL_FS_CREATE | SL_FS_CREATE_FAILSAFE | SL_FS_CREATE_MAX_SIZE(MAX_FILE_SIZE)), 0);
/*	fd = sl_FsOpen((unsigned char *)TEST_FILENAME1,
            SL_FS_CREATE|SL_FS_OVERWRITE| SL_FS_CREATE_MAX_SIZE( MAX_FILE_SIZE ),
            NULL);*/
	if(fd < 0)
	{
		UART_PRINT("sl_FsOpen error11: %d\n\r", fd);
	}
/*	else
	{
		st_writeFile(fd, strlen(originalContent), originalContent);
		RetVal = sl_FsClose(fd, 0, 0, 0);
		if(fd < 0)
		{
			UART_PRINT("sl_FsClose error: %d\n\r", RetVal);
		}
	}*/
    st_listFiles(40,0);

    // Read the file content
    UART_PRINT("Reading file after CREATION:\n\r");
    fd = sl_FsOpen(TEST_FILENAME, SL_FS_READ, 0);
    if(fd < 0)
    {
        UART_PRINT("sl_FsOpen error: %d\n\r", fd);
    }
    else
    {
        st_readFile(fd, MAX_FILE_SIZE);
        RetVal = sl_FsClose(fd, 0, 0, 0);
        if(fd < 0)
        {
            UART_PRINT("sl_FsClose error: %d\n\r", RetVal);
        }
    }

	return RetVal;
}

#endif
int CheckFile_System()
{
	st_ShowStorageInfo();
	st_listFiles(40, 1);
	return 0;
}

int InitFile(unsigned long *ulToken, _i32 lFileHandle)
{
	_i32            RetVal;
//	st_listFiles(40,0);
	SlFsFileInfo_t FsFileInfo;

	RetVal = sl_FsGetInfo(USER_FILE_NAME,0,&FsFileInfo);
	UART_PRINT("sl_FsGetInfo: %d\n\r",RetVal);

	if((sl_FsGetInfo(USER_FILE_NAME,0,&FsFileInfo)) < 0)
	{
		UART_PRINT("Create File\n\r");
		lFileHandle = sl_FsOpen((unsigned char *)USER_FILE_NAME, (SL_FS_CREATE | SL_FS_WRITE  | SL_FS_CREATE_MAX_SIZE(SL_MAX_FILE_SIZE)) \
				, ulToken);
		if(lFileHandle < 0)
		{
			UART_PRINT("sl_FsOpen error InitFile: %d\n\r", lFileHandle);
			RetVal = sl_FsClose(lFileHandle, 0, 0, 0);
			if(lFileHandle < 0)
			{
				UART_PRINT("sl_FsClose error InitFile: %d\n\r", RetVal);
			}
		}
		RetVal = sl_FsWrite(lFileHandle, 0, (unsigned char *)get_userconfig_pointer(), get_userconfig_size()+1);
        if (RetVal < 0){

            RetVal = sl_FsClose(lFileHandle, 0, 0, 0);
            return RetVal;
        }
        RetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        if (SL_RET_CODE_OK != RetVal)
        {
            _ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
        }
	}
	else
	{
		UART_PRINT("File Exists\n\r");
	}
	return 0;
}

long ReadFileFromDevice(unsigned long ulToken, long lFileHandle, unsigned char *data, int len)
{
    long lRetVal = -1;
    //
    // open a user file for reading
    //
    lFileHandle = sl_FsOpen((unsigned char *)USER_FILE_NAME, (SL_FS_READ) ,&ulToken);

    if(lFileHandle < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        _ASSERT_ON_ERROR(FILE_OPEN_READ_FAILED);
    }

    //
    // read the data
    //
    lRetVal = sl_FsRead(lFileHandle,0, data, len);
    if ((lRetVal < 0) || (lRetVal != len))
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
       _ASSERT_ON_ERROR(FILE_READ_FAILED);
    }
    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        _ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return 0;
}
long WriteFileToDevice(unsigned long *ulToken, long lFileHandle, unsigned char* data, int len)
{
    long lRetVal = -1;
    //
    //  open a user file for writing
    //
    lFileHandle = sl_FsOpen((unsigned char *)USER_FILE_NAME, (SL_FS_WRITE), ulToken);
    if(lFileHandle < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        _ASSERT_ON_ERROR(FILE_OPEN_WRITE_FAILED);
    }

    //
    // write the data to file
    //

    lRetVal = sl_FsWrite(lFileHandle, 0, (unsigned char *)data, len);
    if (lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        _ASSERT_ON_ERROR(FILE_WRITE_FAILED);
    }


    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        _ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return 0;
}
