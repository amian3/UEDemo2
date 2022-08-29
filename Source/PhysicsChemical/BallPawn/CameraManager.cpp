// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "BallPawn.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	RootComponent = Camera;


	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectBox"));
	DetectBox->SetupAttachment(Camera);


}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	DetectBox->OnComponentBeginOverlap.AddDynamic(this, &ACameraManager::OnDetectBoxOverlapBegin);
	DetectBox->OnComponentEndOverlap.AddDynamic(this, &ACameraManager::OnDetectBoxOverlapEnd);
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraManager::OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if (CameraSta == CameraStatus::Lock)
			{
				
				PlayerBall->SetCurrentCameraProperty();
				PlayerBall->SetTargetCameraProperty(TargetArmLength, RelativeLocation, RelativeRotation);
				PlayerBall->GetSpringArm()->bUsePawnControlRotation = false;
				PlayerBall->CurrentForwardDirection = TargetForwardDirection;
				PlayerBall->CurrentRightDirection = TargetRightDirection;
				PlayerBall->GetTimeline()->SetPlaybackPosition(0.0f, false);
				PlayerBall->GetTimeline()->Play();


				PlayerBall->CMode = CameraMode::Lock;
				
			}
			else if (CameraSta == CameraStatus::Static)
			{
				Cast<APlayerController>(PlayerBall->GetController())->SetViewTargetWithBlend(this, 3.0f);
				PlayerBall->CurrentForwardDirection = TargetForwardDirection;
				PlayerBall->CurrentRightDirection = TargetRightDirection;
				PlayerBall->CMode = CameraMode::Static;
			}
		}
	}
}

void ACameraManager::OnDetectBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if (CameraSta == CameraStatus::Lock)
			{
			//	PlayerBall
			//	PlayerBall->SetCurrentCameraProperty();
				PlayerBall->GetSpringArm()->bUsePawnControlRotation = true;
				if (PlayerBall->GetController() && PlayerBall->GetController()->GetViewTarget() == PlayerBall)
				{
					PlayerBall->GetController()->SetControlRotation(PlayerBall->GetSpringArm()->GetRelativeRotation());
				}
				PlayerBall->SetCurrentCameraProperty();
				PlayerBall->GetReverseTimeline()->SetPlaybackPosition(0.0f, false);
				PlayerBall->GetReverseTimeline()->Play();
				//	PlayerBall->GetCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			//	PlayerBall->GetSpringArm()->TargetArmLength = PlayerBall->CurrentSpringArmLength;
				//PlayerBall->GetCamera()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
				PlayerBall->CMode = CameraMode::Free;
			}
			else if (CameraSta == CameraStatus::Static)
			{
				if (PlayerBall->GetController() && PlayerBall->GetController()->GetViewTarget())
				{
					Cast<APlayerController>(PlayerBall->GetController())->SetViewTargetWithBlend(PlayerBall, 3.0f);
					PlayerBall->CMode = CameraMode::Free;
				}
			}
		}
	}
}