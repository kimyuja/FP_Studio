// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_SneakyAnimals.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include <DataStructure.h>


UGI_SneakyAnimals::UGI_SneakyAnimals()
{

}

void UGI_SneakyAnimals::Init()
{
    Super::Init();

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnCreateSessionComplete);
            //SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnFindSessionsComplete);
        }
    }

}

void UGI_SneakyAnimals::CreateSession(APlayerController* PlayerController, int32 MaxConnections, bool UseLAN, bool UseLobbies)
{
    if (SessionInterface.IsValid() && PlayerController)
    {
        // Create a new session settings object
        SessionSettings = MakeShareable(new FOnlineSessionSettings());

        // Set your session settings here
        SessionSettings->bIsLANMatch = UseLAN;
        SessionSettings->bUsesPresence = UseLobbies;
        SessionSettings->NumPublicConnections = MaxConnections;
        SessionSettings->bAllowJoinInProgress = true;
        SessionSettings->bShouldAdvertise = true;
        SessionSettings->bAllowJoinViaPresence = true;
        SessionSettings->bIsDedicated = false;

        // 방 번호는 랜덤으로
        FName RandSessionName = GetDynamicSessionName();

        // Create the session
        SessionInterface->CreateSession(*PlayerController->GetLocalPlayer()->GetPreferredUniqueNetId()
            , RandSessionName, *SessionSettings);
    }

}

//void UGI_SneakyAnimals::FindSessions(APlayerController* PlayerController, int32 MaxResults_SessionsToFind, bool bSearchForLAN)
//{
//    if (SessionInterface.IsValid() && PlayerController)
//    {
//        SessionSearch = MakeShareable(new FOnlineSessionSearch());
//        SessionSearch->bIsLanQuery = bSearchForLAN;
//        SessionSearch->MaxSearchResults = MaxResults_SessionsToFind;
//        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//
//        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
//
//        SessionInterface->FindSessions(*PlayerController->GetLocalPlayer()->GetPreferredUniqueNetId(), SearchSettingsRef);
//    }
//}
//
//void UGI_SneakyAnimals::OnFindSessionsComplete(bool bWasSuccessful)
//{
//    if (bWasSuccessful && SessionSearch.IsValid())
//    {
//        TArray<FMyBlueprintSessionResult> BlueprintSessionResults;
//
//        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
//        {
//            FMyBlueprintSessionResult BlueprintSessionResult;
//            BlueprintSessionResult.ServerName = SearchResult.GetSessionIdStr();
//            BlueprintSessionResult.CurrentPlayers = SearchResult.Session.NumOpenPublicConnections;
//            BlueprintSessionResult.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
//            BlueprintSessionResult.PinginMs = SearchResult.PingInMs;
//
//            BlueprintSessionResults.Add(BlueprintSessionResult);
//        }
//
//        OnFindSessionsCompleteCustom.Broadcast(BlueprintSessionResults);
//    }
//    else
//    {
//        TArray<FMyBlueprintSessionResult> EmptyResults;
//        OnFindSessionsCompleteCustom.Broadcast(EmptyResults);
//    }
//}

void UGI_SneakyAnimals::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        OnCreateSessionSuccess.Broadcast();
    }
    else
    {
        OnCreateSessionFailure.Broadcast();
    }
}

FName UGI_SneakyAnimals::GetDynamicSessionName()
{
    FString DynamicName = "Room_" + FString::FromInt(FMath::Rand());
    return FName(*DynamicName);
}
