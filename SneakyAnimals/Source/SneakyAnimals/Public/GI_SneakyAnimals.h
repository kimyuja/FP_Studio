#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "DataStructure.h"
#include "GI_SneakyAnimals.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionFailure);

// new version(net tps gs)
// 방찾기 요청 후 응답이 왔을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchDelegate, const FSessionInfo&, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchFinishedDelegate, bool, bSearching);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UGI_SneakyAnimals : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Shutdown() override;
	virtual void Init() override;

	int32 GetUserIndex(const FString& UserName);
    void RemoveUserIndex(const FString& UserName);
	bool bContains_UserName(const FString& UserName);

	// new version(net tps gs)
	UPROPERTY(BlueprintReadOnly, Category = "Session")
    FSessionInfo S_SessionInfo;
	
	// 세션 인터페이스를 만들고싶다.
	IOnlineSessionPtr sessionInterface;

	FSessionSearchDelegate OnMySessionSearchCompleteDelegate;

	FSessionSearchFinishedDelegate OnMySessionSearchFinishedDelegate;

	// 방생성 요청 기능
    void CreateSession(FString roomName, int32 playerCount);

	// 방생성 응답
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	FString mySessionName = TEXT("Sneaky_Animals");

	// 방을 찾고 싶다.
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	
	void FindOtherSessions();

	void OnFindSessionsComplete(bool bWasSuccessful);

	// 방에 조인하고 싶다.
	void JoinMySession(int32 index);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void ExitRoom(FString DeleteUserName);

	//bool IsInRoom();

	//UFUNCTION(Server, Reliable)
	//void ServerExitRoom();

	//UFUNCTION(NetMulticast, Reliable)
	//void MultiExitRoom();

	void OnMyExitRoomComplete(FName sessionName, bool bWasSuccessful);

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);

	UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnCreateSessionSuccess OnCreateSessionSuccess;

    UPROPERTY(BlueprintAssignable, Category = "Network")
    FOnCreateSessionFailure OnCreateSessionFailure;

	FText MyName;
	int32 KickCount = 0;

//--------------------
	
	UFUNCTION()
	void GetRandomplayer();

	class ATestPlayer* ranPlayer;

	TArray<class ATestPlayer*> players;

//--------------------
private:
	TMap<FString, int32> UserIndexMap;
	void LoadUserIndexMap();
	void SaveUserIndexMap();
	void InitializeUserIndexJson();

};
