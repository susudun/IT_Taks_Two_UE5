// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemActor/CheckPointData.h"
#include "Engine/GameInstance.h"
#include "ITTGameInstance.generated.h"

class ACheckPoint;
DECLARE_LOG_CATEGORY_EXTERN(LogGIS, Log, All)


UCLASS()
class PROJECT_ITT_API UITTGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Audio")
	UDataTable* AudioDataTable;
	
	virtual void Init() override;
#pragma region Variables
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CheckPoint", meta=(AllowPrivateAccess="true"))
	uint8 InitCheckPointIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CheckPoint", meta=(AllowPrivateAccess="true"))
	uint8 CurrentCheckPointIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CheckPoint", meta=(AllowPrivateAccess="true"))
	ACheckPoint* CurrentCheckPoint;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CheckPoint")
	TMap<uint8, FCheckPointData> CheckPointMap;
public:
	//WidgetUserName
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "WidgetUserName")
	FString CachedUserName;
#pragma endregion
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	void RegisterCheckPoint(ACheckPoint* SpawnedCheckPoint);

	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	void ActivateCheckPoint(uint8 CheckPointIndex);

	UFUNCTION()
	void RespawnPlayerCharacter(APlayerController* PC);
#pragma endregion
#pragma region DEBUG
public:
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	bool GetCheckPointIsActivateByID(uint8 InID);
	
	UPROPERTY()
	AActor* DeadActor;

	UFUNCTION()
	FORCEINLINE void SetDeadActor(AActor* InActor) {DeadActor = InActor;}
	
#pragma endregion
};
