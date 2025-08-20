// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharaterChoice.generated.h"

class ABasePlayerController;
class UImage;
class UButton;
class UTextBlock;

DECLARE_LOG_CATEGORY_EXTERN(PlayerCharaterChoiceWidget, Log, All)
/**
 * 
 */
UCLASS()
class PROJECT_ITT_API UPlayerCharaterChoice : public UUserWidget
{
	GENERATED_BODY()
	UPlayerCharaterChoice(const FObjectInitializer& ObjectInitializer);
#pragma region Key,ButtenEvent Functions
private:
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintCallable)
	void LeftKeyEvent();
	UFUNCTION(BlueprintCallable)
	void RightKeyEvent();
	UFUNCTION()
	void GameStartingButtenEvent();
#pragma endregion
#pragma region Binding
public:
	UPROPERTY(meta=(BindWidgetOptional))UImage* Client_0;
	UPROPERTY(meta=(BindWidgetOptional))UImage* Client_1;
	UPROPERTY(meta=(BindWidget))UButton* GameStart;
	UPROPERTY(meta=(BindWidget))UTextBlock* Player_Name;
	UPROPERTY(meta=(BindWidget))UTextBlock* Player_Name_1;
#pragma endregion
#pragma region Functions
	void WidgetEixt(ABasePlayerController* PC);
	void SpotLightCodyToGoMay();
	void SpotLightMayToGoCody();
#pragma endregion
#pragma region Variables
public:
	bool bInGameStart = false;
private:
	// 로드한 사운드 애셋을 저장할 변수
	UPROPERTY()
	USoundWave* GameStartSound;
#pragma endregion
};
