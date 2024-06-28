// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Clock.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_Clock::NativeConstruct()
{
	Super::NativeConstruct();

	if (Time_Text)
	{
		Time_Text->TextDelegate.BindDynamic(this, &UW_Clock::Get_Time);
	}
}

FText UW_Clock::Get_Time()
{
	FDateTime t = UKismetMathLibrary::Now();

	FString h = UKismetStringLibrary::Conv_IntToString(t.GetHour());
	FString m = UKismetStringLibrary::Conv_IntToString(t.GetMinute());
	FString s = UKismetStringLibrary::Conv_IntToString(t.GetSecond());
	FString separator = ":";

	FString StringResult;
	StringResult.Empty(h.Len() + separator.Len() + m.Len() + separator.Len() + s.Len() + 1); // adding one for the string terminator
	StringResult += h;
	StringResult += separator;
	StringResult += m;
	StringResult += separator;
	StringResult += s;

	return UKismetTextLibrary::Conv_StringToText(StringResult);

}
