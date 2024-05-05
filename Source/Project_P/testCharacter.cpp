// Fill out your copyright notice in the Description page of Project Settings.


#include "testCharacter.h"

// Sets default values
AtestCharacter::AtestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera setup
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("mainCamera"));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	Camera->SetRelativeRotation(FQuat(0.0f));
	Camera->SetupAttachment(RootComponent);

	//Player Controller setup
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	//Physics Handle setup
	PhyHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("phyHandle"));
	
	//walkSFX setup
	WalkSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("walkSFX"));
	WalkSFX->SetRelativeLocation(FVector(0.0f, 0.0f, -70.0f));
	WalkSFX->SetupAttachment(RootComponent);

	HeartSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("heartSFX"));
	HeartSFX->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	HeartSFX->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AtestCharacter::BeginPlay()
{
	Super::BeginPlay();
	gameWorld = GetWorld();

	// Set the base speed for the character (600 is base speed)
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	saveYeetForce = YeetForce;
	saveFieldOfView = Camera->FieldOfView;

	adoclass = UGameplayStatics::GetActorOfClass(gameWorld, AadoChild::StaticClass());
	ado = Cast<AadoChild>(adoclass);

	// Check value
	if (stressBPM > 100.0f) { stressBPM = 100.0f; }
	if (YeetForce > 800.0f) { YeetForce = 800.0f; }

	//Camera->SetRelativeRotation(FQuat(0.0f));
	this->SetActorRotation(Camera->GetRelativeRotation());
	
}

// Called every frame
void AtestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug message
	{
		FString rotPlayer = this->CameraInput.ToString();
		FString rotCam = Camera->GetRelativeRotation().ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, rotPlayer);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, rotCam);
	}

	//camera 
	{
		//Look left/right
		{
			FRotator NewRotationX = GetActorRotation();
			NewRotationX.Yaw = CameraInput.X;
			SetActorRotation(NewRotationX);
		}

		//Look up/down
		{
			FRotator NewRotationY = Camera->GetRelativeRotation();
			// Limit the rotation of the camera (not do a barrel roll)
			NewRotationY.Pitch = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
			Camera->SetRelativeRotation(NewRotationY);
		}
	}

	if (isShoked == true && humainForm) {
		countShoked += DeltaTime;
		
		if(stressBPM <= 100) stressBPM += 0.3f;
		
		if (countShoked >= maxCountShoked) {
			isShoked = false;
			countShoked = 0.0f;
		}
	}

	// Stress jauge
	{
		if (ado != nullptr && humainForm) {
			distance = GetDistanceTo(ado);
			//GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Blue, FString::SanitizeFloat(distance));
		
			if (distance <= maxDistanceAdo && stressBPM <= 100) {
				stressBPM += 0.5;
			}
			/*if (distance >= maxDistanceAdo && stressBPM >= 0) {
				stressBPM -= 0.5;
			}*/
		
		}
		
		if (stressBPM >= 0)stressBPM -= 0.01;

		if (isRuning == true && humainForm) {
			countHide += DeltaTime;
			if (countHide >= maxCountHide) {
				stressBPM += 0.05f;
			}
		}

		if (isHide == true && humainForm) {
			countRuning += DeltaTime;
			if (countRuning >= maxCountRuning) {
				stressBPM += 0.08f;
			}
		}
	}

	// Camera FX
	{
		if (humainForm) {
			Camera->PostProcessSettings.VignetteIntensity = 3 * stressBPM * 0.01;
			Camera->PostProcessSettings.FilmGrainIntensity = 10 * stressBPM * 0.01;
		}
	}

	// Grab system
	{
		if (isGrabed) {
			PhyHandle->SetTargetLocation(Camera->GetForwardVector() * 100 + Camera->GetComponentLocation());
			//PhyHandle->SetTargetLocationAndRotation(Camera->GetForwardVector() * 100 + Camera->GetComponentLocation(), GetActorRotation());
		}
	}

	// Sound FX
	{
		// Footstep 
		{
			WalkSFX->Sound = footsteps[numFootstep];

			if (isWalkingF || isWalkingD) {
				footstepcount += DeltaTime;
				if (isRuning) { footstepcount += DeltaTime; }

				if (footstepcount >= maxFootstepCount) {

					//numFootstep = FMath::RandRange(0, footsteps.Num() - 1);
					numFootstep += 1;
					if (numFootstep >= footsteps.Num()) { numFootstep = 0; }
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(numFootstep));

					WalkSFX->Play();
					footstepcount = 0.0f;
				}
			}
		}

		// Breathing || stressBPM 100 max
		{
			if (humainForm) {
				HeartSFX->Sound = heartBeat[numHeartbeat];
				heartbeatCount += DeltaTime;
			}
			else {
				HeartSFX->Sound = Breathing[numHeartbeat];
				heartbeatCount += DeltaTime/4;
			}

			actualHeartBeatCount = maxHeartbeatCount - 0.7f * (stressBPM / 100.0f);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(actualHeartBeatCount));


			if (heartbeatCount >= actualHeartBeatCount) {
				numHeartbeat += 1;

				if (humainForm) if (numHeartbeat >= heartBeat.Num()) { numHeartbeat = 0; }
				if (!humainForm) if (numHeartbeat >= Breathing.Num()) { numHeartbeat = 0; }

				HeartSFX->Play();
				heartbeatCount = 0.0f;
			}
		}
	}

	if (!isGrabed && grabChild) {
		CountGrab += DeltaTime;
		if (CountGrab >= 1.0f) {
			CountGrab = 0.0f;
			grabChild = false;
		}
	}

	if( stressBPM >= 100 ){
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(countTransform));

	if (goTransform) {
		countTransform += DeltaTime * 2;
		if (!humainForm) {
			Camera->FieldOfView = 90 + (20 * countTransform);
			Camera->PostProcessSettings.VignetteIntensity = countTransform;
			Camera->PostProcessSettings.AutoExposureBias = 3.0f * countTransform;
			if (countTransform >= 1.0f) {
				countTransform = 0.0f;
				goTransform = false;
			}
		} 
		if (humainForm){
			Camera->FieldOfView = 110 - (20 * countTransform);
			Camera->PostProcessSettings.VignetteIntensity = 1.0f - countTransform;
			Camera->PostProcessSettings.AutoExposureBias = 3.0f - 3.0f * countTransform;
			if (countTransform >= 1.0f) {
				countTransform = 0.0f;
				goTransform = false;
			}
		}
	}
}


void AtestCharacter::MoveForward(float value) {
	if (isShoked == true) { return; }
	AddMovementInput(GetActorForwardVector(), value);
	if (value == -1.0f || value == 1.0f) { isWalkingF = true; }
	else { isWalkingF = false; }

}

void AtestCharacter::MoveSide(float value) {
	if (isShoked == true) {	return; }
	AddMovementInput(GetActorRightVector(), value);
	if (value == -1.0f || value == 1.0f) { isWalkingD = true; }
	else { isWalkingD = false; }
}

void AtestCharacter::MoveRunBegin() {
	//Code for run action
	if (isShoked == true || isGrabed == true) { return; }

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed * runSpeed;
	if (!humainForm) GetCharacterMovement()->MaxWalkSpeed = walkSpeed * runSpeed * 3;
	isRuning = true;
	
}

void AtestCharacter::MoveRunEnd() {
	//Code for run action

	if (isShoked == true || isGrabed == true) { return; }
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	if (!humainForm) GetCharacterMovement()->MaxWalkSpeed = walkSpeed * 3;
	isRuning = false;
	countRuning = 0.0f;
}

void AtestCharacter::LookUp(float value) {
	if (isShoked == false) CameraInput.Y += value * cameraSpeed;
	// Limit the rotation of the camera (not do a barrel roll)
	CameraInput.Y = FMath::Clamp(CameraInput.Y, -80.0f, 80.0f);
}

void AtestCharacter::LookSide(float value) {
	if (isShoked == false) CameraInput.X += value * cameraSpeed;
	if (CameraInput.X < -360 || CameraInput.X > 360) { CameraInput.X = 0; }
}

void AtestCharacter::Yeet() {
	//Code for begin hide action
	if (isGrabed) {
		//PhyHandle->ReleaseComponent();
		//isGrabed = false;
		UPrimitiveComponent* grabTemp = PhyHandle->GrabbedComponent;

		PhyHandle->ReleaseComponent();
		grabTemp->AddForce(FVector(Camera->GetForwardVector() * YeetForce * 1000.0f * grabTemp->GetMass()));

		grabChild = false;
		isGrabed = false;
	}
}

void AtestCharacter::HideEnd() {
	//Code for exit hide action
}

void AtestCharacter::GrabBegin() {
	Grabing();
}

void AtestCharacter::GrabEnd() {
	// Drop the item
	if (isGrabed) {
		if(humainForm) GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		//PhyHandle->ReleaseComponent();
		//isGrabed = false;
		UPrimitiveComponent* grabTemp = PhyHandle->GrabbedComponent;
		
		PhyHandle->ReleaseComponent();
		grabTemp->AddForce(FVector(Camera->GetForwardVector() * 1000.0f* grabTemp->GetMass()));
		
		isGrabed = false;
		//grabChild = false;
	}

}

AActor* AtestCharacter::Grabing() {
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	FCollisionResponseParams collisionResponse;

	collisionParams.AddIgnoredActor(this);

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Grab clic")));


	// Cast a single Line trace face of the cam 
	if (gameWorld->LineTraceSingleByChannel(hitResult, Camera->GetComponentLocation(), Camera->GetForwardVector() * 500 + Camera->GetComponentLocation(), ECC_WorldStatic, collisionParams, collisionResponse))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *hitResult.GetComponent()->GetName()));

		UPrimitiveComponent* ComponentToGrab = hitResult.GetComponent();

		PhyHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, hitResult.Location);

		//Grab item
		if ((*hitResult.GetComponent()->GetName() == FString("Cube") ||
			*hitResult.GetComponent()->GetName() == FString("Sac")) &&
			PhyHandle->GrabbedComponent && humainForm) {
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed/2;
			//PhyHandle->SetTargetLocation(hitResult.Location);
			isGrabed = true;
		}

		if (*hitResult.GetComponent()->GetName() == FString("CollisionCylinder") &&
			PhyHandle->GrabbedComponent && !humainForm) {
			//PhyHandle->SetTargetLocation(hitResult.Location);
			isGrabed = true;
			grabChild = true;
		}
		
		//open door item
		if (*hitResult.GetComponent()->GetName() == FString("Porte_Circle")){
			return hitResult.GetActor();
		}
	}

	return nullptr;
}

void AtestCharacter::Shoked(FRotator childRotation) {

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, childRotation.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, this->GetActorRotation().ToString());

	FRotator newLook = childRotation - FRotator(0, 180, 0);


	CameraInput.X = newLook.Yaw;

	SetActorRotation(newLook);

	FRotator NewRotationX = GetActorRotation();
	NewRotationX.Yaw = CameraInput.X;
	SetActorRotation(NewRotationX);

	isShoked = true;
}

void AtestCharacter::changeBPM(bool add, float number) {
	if (add == true) {
		stressBPM += number;
		if (stressBPM >= 100.0f) { stressBPM = 100.0f; }
	}
	else {
		stressBPM -= number;
		if (stressBPM <= 0.0f) { stressBPM = 0.0f; }
	}
}

bool AtestCharacter::getIsHide() { return isHide; }

void AtestCharacter::setIsHide(bool hide) { isHide = hide; }

AActor* AtestCharacter::hideChar(AActor* choseOne) {
	UPrimitiveComponent* hideComponent;

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	FCollisionResponseParams collisionResponse;

	collisionParams.AddIgnoredActor(this);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Hide clic")));


	// Cast a single Line trace face of the cam 
	if (gameWorld->LineTraceSingleByChannel(hitResult, Camera->GetComponentLocation(), Camera->GetForwardVector() * 500 + Camera->GetComponentLocation(), ECC_WorldStatic, collisionParams, collisionResponse))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *hitResult.GetComponent()->GetName()));

		hideComponent = hitResult.GetComponent();

		//PhyHandle->GrabComponentAtLocation(hideComponent, NAME_None, hitResult.Location);

		//Hide item
		if (*hitResult.GetComponent()->GetName() == FString("Table") || 
			*hitResult.GetComponent()->GetName() == FString("porte1") ||
			*hitResult.GetComponent()->GetName() == FString("porte2")){

			return hitResult.GetActor();
		}
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Not a waiting component as ben hit !")));
	return nullptr;
}


// Called to bind functionality to input
void AtestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Move
	PlayerInputComponent->BindAxis("ForwardAxis", this, &AtestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("SideAxis", this, &AtestCharacter::MoveSide);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AtestCharacter::MoveRunBegin);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AtestCharacter::MoveRunEnd);


	// Look
	PlayerInputComponent->BindAxis("LockPitchAxis", this, &AtestCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookYawAxis", this, &AtestCharacter::LookSide);

	// Hide
	//PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &AtestCharacter::Yeet);
	PlayerInputComponent->BindAction("Hide", IE_Released, this, &AtestCharacter::HideEnd);

	// Grab
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AtestCharacter::GrabBegin);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &AtestCharacter::GrabEnd);

	//Shit code
	PlayerInputComponent->BindAction("resetBPM", IE_Pressed, this, &AtestCharacter::ResetBPM);
	PlayerInputComponent->BindAction("ChangeForm", IE_Pressed, this, &AtestCharacter::ChangeForm);
}

void AtestCharacter::ResetBPM() {
	stressBPM = 0.0f;
}

void AtestCharacter::ChangeForm() {
	humainForm = !humainForm;
	if (humainForm == false) {
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed * 3;
		YeetForce = 1500;
		//Camera->FieldOfView = 110;
		//Camera->PostProcessSettings.VignetteIntensity = 1.0f;
		//Camera->PostProcessSettings.AutoExposureBias = 3.0f;
		goTransform = true;
		stressBPM = 0.1f;
		isShoked = false;
		numHeartbeat = 0;
	} else {
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		YeetForce = saveYeetForce;
		//Camera->FieldOfView = saveFieldOfView;
		//Camera->PostProcessSettings.VignetteIntensity = 0;
		//Camera->PostProcessSettings.AutoExposureBias = 0.0f;
		goTransform = true;
		stressBPM = 1.0f;
		numHeartbeat = 0;
	}
}