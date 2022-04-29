#pragma once

#include <HAPI.h>
#include <string>

#include "HEMAX_Types.h"

class HEMAX_HAPISession : public HAPI_Session
{
    public:

	HEMAX_HAPISession();

	virtual ~HEMAX_HAPISession();

	virtual bool CreateSession();

	bool InitializeHAPI(const char* HoudiniEnvFiles,
                            const char * otlSearchPath,
                            const char* dsoSearchPath,
                            const char* imageDsoSearchPath,
                            const char* audioDsoSearchPath);

        HAPI_CookOptions* GetCookOptions() { return &CookOptions; }

	bool LoadHoudiniDigitalAsset(const char* FilePath,
                                     bool AllowOverwrite,
                                     HAPI_AssetLibraryId* AssetLibId,
                                     HEMAX_AssetLoadStatus* LoadStatus=nullptr);

	HEMAX_CookResult IsCookFinished();

	std::string GetHAPIString(HAPI_StringHandle Handle);

	std::string GetHAPIErrorStatusString();

    protected:

	HAPI_CookOptions CookOptions;

	bool SeparateCookingThread;
	int CookingThreadStackSize;

};

class HEMAX_HAPIThriftSocketSession : public HEMAX_HAPISession
{
    public:

	HEMAX_HAPIThriftSocketSession();
	~HEMAX_HAPIThriftSocketSession() override;

	bool CreateSession() override;
	void SetHostName(std::string HName);
	void SetPortNumber(int PortNum);

    private:

	std::string HostName;
	int Port;
};

class HEMAX_HAPIThriftPipeSession : public HEMAX_HAPISession
{
    public:

	HEMAX_HAPIThriftPipeSession();
	~HEMAX_HAPIThriftPipeSession() override;

	bool CreateSession() override;
	void SetPipeName(std::string Name);

    private:

	std::string PipeName;
};
