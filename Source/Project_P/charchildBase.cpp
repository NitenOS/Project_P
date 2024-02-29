// Fill out your copyright notice in the Description page of Project Settings.


#include "charchildBase.h"

// Sets default values
AcharchildBase::AcharchildBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AcharchildBase::BeginPlay()
{
	Super::BeginPlay();

	originPosition = GetActorLocation();
	MoveIA();
}

// Called every frame
void AcharchildBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveIA();
}

// Called to bind functionality to input
void AcharchildBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AcharchildBase::MoveIA() {
	//FPathFollowingRequestResult moveResult;
	FAIMoveRequest moveRequest = FAIMoveRequest();

	aiController = this->GetController<AAIController>();

	FNavLocation goalLocation = FNavLocation(originPosition);
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld()); //Maybe il va pas trouvé qq chose

	navSystem->GetRandomReachablePointInRadius(originPosition, 100.0f, goalLocation);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, goalLocation.Location.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, originPosition.ToString());

	moveRequest.SetGoalLocation(goalLocation.Location);
	aiController->MoveTo(moveRequest);
}

//UNavigationSystem* navSystem = UNavigationSystem::GetCurrent(GetWorld());
//FNavLocation endPosi = FNavLocation(originPosition);
//navS
