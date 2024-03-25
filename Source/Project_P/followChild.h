// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "childBase.h"
#include "testCharacter.h"
#include "Components/CapsuleComponent.h" 
#include "followChild.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_P_API AfollowChild : public AchildBase
{
	GENERATED_BODY()
	
public:
	AfollowChild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	FVector goBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	FVector goHidle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	bool isChasing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	TArray<USoundBase*> footsteps;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	AtestCharacter* mc;

	bool searchPlayer = true;

	// footstep var
	int numFootstep = 0;
	float footstepcount = 0.0f;
	float maxFootstepCount = 0.3f;


	bool goChild = false;
	float countGo = 0.0f;
	float maxCountGo = 2.0f;
 
	bool FollowPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
	UAudioComponent* MusicChase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPPSound)
	UAudioComponent* WalkSFX;

	UCapsuleComponent* capsulComponent = GetCapsuleComponent();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
