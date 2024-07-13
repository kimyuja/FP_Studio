// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GimmickSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UGimmickSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* DieAloneBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* DieAllBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ClearBtn;

	// ��� ��ư �̷��� �� ������ �ִ°���? ���ʹ�
};
