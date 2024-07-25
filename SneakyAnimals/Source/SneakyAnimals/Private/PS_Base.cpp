// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "FL_General.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "GI_SneakyAnimals.h"


void APS_Base::ServerRPC_Update_Player_Appearance_Implementation(const FStructure_Player_Appearance _Player_Appearance)
{
	Player_Appearance = _Player_Appearance;
	// 클라이언트 뿐만 아니라 서버에서도 OnRep 호출하려면 아래처럼 수동으로 호출해줘야한다.
	OnRep_Player_Appearance();
}

void APS_Base::ServerRPC_Update_Player_UserProfile_Implementation(const FStructure_UserProfile _Player_UserProfile)
{
	// 만약 여기서 에러나면 if (HasAuthority()) 붙이기
	Player_UserProfile = _Player_UserProfile;
	OnRep_Player_UserProfile();

}

void APS_Base::ServerRPC_Update_Player_ConnectionInfo_Implementation(const FStructure_PlayerConnectionInfo _Player_ConnectionInfo)
{
	// 만약 여기서 에러나면 if (HasAuthority()) 붙이기
	Player_ConnectionInfo = _Player_ConnectionInfo;
	OnRep_Player_ConnectionInfo();

}

void APS_Base::ClientRPC_Init_Implementation()
{
	// 자식 클래스에서 오버라이드 할 것
}

void APS_Base::Load_Player_Appearance()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Player_Appearance_Implementation(result.Player_Appearance);
		return;
	}
	else
	{
		return;
	}

}

void APS_Base::Load_Player_UserProfile()
{
	// log : client는 rty, server는 kyj -> rty를 kyj 서버가 알게 해야 함
	FUserProfileResult result = UFL_General::Get_UserProfile();

	// KYJ Test
	// 인덱스 0번 세이브 게임
	// json 파일에서, 0번 인덱스는 무조건 서버여야 함(유저네임 주의, 아예 없앤 상태로 시작하는 게 베스트)

	// result에 있는 값을 유니크 인덱스인 세이브 게임에 다시 저장함
	// 다시 저장하는 걸 server rpc로 하면 서버에서도 클라이언트의 세이브 게임이 보임! (유니크 인덱스로 접근)
	// 게임 맵으로 이동 후 FUserProfileResult result = UFL_General::Get_UserProfile(); 0번 인덱스
	// 0번 인덱스에 클라이언트 본인의 유저 네임이 있으니 그걸로 json 에서 유니크 인덱스 찾은 다음 appearance 입히면 됨
	
	// 예외 처리
	UGI_SneakyAnimals* GameInstance = Cast<UGI_SneakyAnimals>(GetGameInstance());

	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance or invalid cast to UGI_SneakyAnimals."));
		return;
	}

	FString Username = result.S_UserProfile.Username.ToString();
	int32 idx = -1;

	try
	{
		idx = GameInstance->GetUserIndex(Username);
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Exception occurred while getting user index: %s"), *FString(e.what()));
		return;
	}
	if (idx == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("User index not found for username: %s"), *Username);
		return;
	}

	//int32 idx = Cast<UGI_SneakyAnimals>(GetGameInstance())->GetUserIndex(result.S_UserProfile.Username.ToString());
	ServerRPC_Update_SaveGame_Player_UserProfile(idx, result.S_UserProfile);

	// idx == 0 이면 서버
	//if (idx == 0 && !HasAuthority()) // server만 나옴
	//if (idx == 0) // client -> server로 전염
	if (idx != GetWorld()->GetGameState()->PlayerArray.Num()-1 + Cast<UGI_SneakyAnimals>(GetGameInstance())->KickCount && HasAuthority()) // 잘 되는데 강퇴하면 크래쉬 남
	{
		UE_LOG(LogTemp, Warning, TEXT("Kickcount : %d"), Cast<UGI_SneakyAnimals>(GetGameInstance())->KickCount);
		// 다시 해 idx 맞을 때 까지
		FTimerHandle t;
		GetWorld()->GetTimerManager().SetTimer(t, [&]() {
			APS_Base::Load_Player_UserProfile();
			return;
			}, 0.2f, false);
	}
	else {
		// player state 에게 맞는 인덱스라면...
		if (HasAuthority())
		{
			// 서버면 인덱스 0 가져와
			// KYJ Test 이거 해보고 안 되면 주석 처리 하기
			result = UFL_General::Get_UserProfile_with_idx(0);
		} 
		else
		{
			// 클라이언트면 고유 인덱스로 가져와
			result = UFL_General::Get_UserProfile_with_idx(idx);
		}
		if (!result.S_UserProfile.Username.IsEmpty())
		{
			// 각 player state 별로 my name 기억해두기
			SetPlayerName(result.S_UserProfile.Username.ToString());
		}
	}


	if (result.success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Player User Profile : %s"), *result.S_UserProfile.Username.ToString());
		ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
		return;
	}
	else
	{
		FStructure_UserProfile tmp;
		tmp.Username = FText::FromString(TEXT("Username"));
		tmp.User_Avatar = T_ProfilePicture;
		ServerRPC_Update_Player_UserProfile_Implementation(tmp);
		return;
	}
}

void APS_Base::Load_Player_ConnectionInfo(bool ClientReadyStatus)
{
	// Sets Player Connection Info incase we are host
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		// The host is always ready, so set ready status to true.
		FStructure_PlayerConnectionInfo p;
		p.bIsHost = true;
		p.bReadyStatus = true;
		p.PlayerID = 0;
		ServerRPC_Update_Player_ConnectionInfo(p);
	}
	// Sets Player Connection Info incase we are a client
	else
	{
		FStructure_PlayerConnectionInfo p;
		p.bIsHost = false;
		// ClientReadyStatus : This variable gets set by the GM_Lobby variable called 'CanHostForceLaunch'
		p.bReadyStatus = ClientReadyStatus;
		p.PlayerID = 0;
		ServerRPC_Update_Player_ConnectionInfo(p);
	}

}

APS_Base::APS_Base()
{
	// Loading Screen Texture2D 경로 설정
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/Placeholders/T_ProfilePicture_Placeholder.T_ProfilePicture_Placeholder"));

	// Loading Screen Texture2D 로드
	T_ProfilePicture = Cast<UTexture2D>(TextureRef.TryLoad());
}

void APS_Base::BeginPlay()
{
	Super::BeginPlay();

	// This initialization event can be overridden in Child Player States
	ClientRPC_Init();
}

void APS_Base::OnRep_Player_Appearance()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_Appearance_OR()를 호출해야 함
	OnRep_Player_Appearance_OR();
}

void APS_Base::OnRep_Player_UserProfile()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_UserProfile_OR()를 호출해야 함
	OnRep_Player_UserProfile_OR();
}

void APS_Base::OnRep_Player_ConnectionInfo()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_ConnectionInfo_OR()를 호출해야 함
	OnRep_Player_ConnectionInfo_OR();
}


void APS_Base::OnRep_Player_Appearance_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_Appearance_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::OnRep_Player_UserProfile_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_UserProfile_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::OnRep_Player_ConnectionInfo_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_ConnectionInfo_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::ServerRPC_Update_SaveGame_Player_UserProfile_Implementation(int32 uniqueIdx, const FStructure_UserProfile _Player_UserProfile)
{
	UFL_General::Save_UserProfile_with_idx(uniqueIdx, _Player_UserProfile);
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
	DOREPLIFETIME(APS_Base, Player_UserProfile);
	DOREPLIFETIME(APS_Base, Player_ConnectionInfo);
}