// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "childBase.h"
#include "Components/AudioComponent.h"
#include "adoChild.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API AadoChild : public AchildBase
{
	GENERATED_BODY()
	
public:
	AadoChild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	TArray<USoundBase*> cry;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP Seting")
	float adoWaitTime = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP Seting")
	float test;

	int navPointCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
	UAudioComponent* crySFX;

	// footstep var
	int numCry = 0;
	float crycount = 0.0f;
	float maxCryCount = 5.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
