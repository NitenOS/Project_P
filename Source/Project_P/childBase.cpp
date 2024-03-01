// Fill out your copyright notice in the Description page of Project Settings.


#include "childBase.h"

// Sets default values
AchildBase::AchildBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AchildBase::BeginPlay()
{
	Super::BeginPlay();



	originPosition = GetActorLocation();
	goalLocation = FNavLocation(originPosition);
	OnMoveCompleted(MoveIA(goalLocation));

	navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	navSystem->GetRandomReachablePointInRadius(originPosition, 100.0f, goalLocation);	
	
}

// Called every frame
void AchildBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//OnMoveCompleted(MoveIA(goalLocation));
	//GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity + FVector(10000);
}

// Called to bind functionality to input
void AchildBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FPathFollowingRequestResult AchildBase::MoveIA(FNavLocation _goalLocation) {
	FPathFollowingRequestResult moveResult;
	FAIMoveRequest moveRequest = FAIMoveRequest();

	aiController = this->GetController<AAIController>();

	moveRequest.SetGoalLocation(_goalLocation.Location);
	aiController->MoveTo(moveRequest);
	return aiController->MoveTo(moveRequest);
}

void AchildBase::OnMoveCompleted(EPathFollowingRequestResult::Type Result)
{
	switch (Result)
	{
	case EPathFollowingRequestResult::Failed:
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Ca doit pas marcher !")));
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Purple, FString(TEXT("Ca a trouvé ou marcher !")));
		//GetWorldTimerManager().SetTimer(TimerHandle, On);
		//GetWorldTimerManager().SetTimer(TimerHandle, this, OnMoveCompleted(MoveIA(goalLocation)), 1.0f, false, 2.0f);
		navSystem->GetRandomReachablePointInRadius(originPosition, 100.0f, goalLocation);
		OnMoveCompleted(MoveIA(goalLocation));
		break;
	case EPathFollowingRequestResult::RequestSuccessful:
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(TEXT("Ca a marcher !")));
		break;
	}
}