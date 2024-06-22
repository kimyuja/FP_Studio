// Fill out your copyright notice in the Description page of Project Settings.


#include "FL_General.h"
#include "SG_UserProfile.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

bool UFL_General::Save_UserProfile(const FStructure_UserProfile& S_UserData)
{
	// Save Game 생성
	USG_UserProfile* SG_UserProfile_inst = Cast<USG_UserProfile>(UGameplayStatics::CreateSaveGameObject(USG_UserProfile::StaticClass()));

	// User Data 저장
	SG_UserProfile_inst->S_UserProfile = S_UserData;

	// 저장 슬롯에 데이터를 저장
	return UGameplayStatics::SaveGameToSlot(SG_UserProfile_inst, TEXT("UserProfile_Slot"), 0);

}

FUserProfileResult UFL_General::Get_UserProfile()
{
    FUserProfileResult result;

    if (UGameplayStatics::DoesSaveGameExist(TEXT("UserProfile_Slot"), 0))
    {
        // UserProfile_Slot 이 있으면 S_UserProfile과 bool success = true 리턴하기
        USG_UserProfile* sg = Cast<USG_UserProfile>(UGameplayStatics::LoadGameFromSlot(TEXT("UserProfile_Slot"), 0));
        if (sg)
        {
            result.S_UserProfile = sg->S_UserProfile;
            result.success = true;
            return result;
        }
    }

    // UserProfile_Slot 이 없으면 비어있는 S_UserProfile과 bool success = false 리턴하기
    result.S_UserProfile = FStructure_UserProfile();
    result.success = false;
    return result;
}