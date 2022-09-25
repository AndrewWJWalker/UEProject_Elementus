// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEProject_Elementus

#include "GAS/Tasks/PEAim_Task.h"
#include "Actors/Character/PECharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"

UPEAim_Task::UPEAim_Task(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = false;
}

UPEAim_Task* UPEAim_Task::Aim(UGameplayAbility* OwningAbility, const FName TaskInstanceName, const FVector CameraRelativeTargetPosition, const float CameraLerpTime, const bool bAdjustTimeToCurrentLocation)
{
	UPEAim_Task* const MyObj = NewAbilityTask<UPEAim_Task>(OwningAbility, TaskInstanceName);	
	MyObj->CameraTargetPosition = CameraRelativeTargetPosition;
	MyObj->CameraLerpTime = CameraLerpTime;
	MyObj->bAdjustTimeToCurrentLocation = bAdjustTimeToCurrentLocation;
	
	return MyObj;
}

void UPEAim_Task::Activate()
{
	Super::Activate();

	check(Ability);

	TaskTimeline = NewObject<UTimelineComponent>();
	if (!TaskTimeline.IsValid())
	{
		EndTask();
		return;
	}

	APECharacter* const OwningCharacter = Cast<APECharacter>(Ability->GetAvatarActorFromActorInfo());

	if (ensureAlwaysMsgf(IsValid(OwningCharacter), TEXT("%s have a invalid Owner"), *GetName()))
	{
		TargetCamera = OwningCharacter->GetFollowCamera();
		CameraInitialPosition = TargetCamera->GetRelativeLocation();

		if (!TargetCamera.IsValid())
		{
			EndTask();
		}

		if (bAdjustTimeToCurrentLocation)
		{
			const float Distance1 = FVector::Distance(CameraInitialPosition, CameraTargetPosition);
			const float Distance2 = FVector::Distance(APECharacter::GetCameraDefaultPosition(), CameraTargetPosition);
			CameraLerpTime *= (Distance1 / Distance2);
		}
		
		TaskTimeline->SetTimelineLength(CameraLerpTime);

		UCurveFloat* const TimelineCurve = NewObject<UCurveFloat>();
		TimelineCurve->FloatCurve.AddKey(0.f, 0.f);
		TimelineCurve->FloatCurve.AddKey(CameraLerpTime, 1.f);

		FOnTimelineFloat TimelineUpdate;
		TimelineUpdate.BindUFunction(this, TEXT("TimelineProgress"));
		TaskTimeline->AddInterpFloat(TimelineCurve, TimelineUpdate);
		
		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, TEXT("TimelineFinished"));
		TaskTimeline->SetTimelineFinishedFunc(TimelineFinished);
		
		TaskTimeline->RegisterComponentWithWorld(GetWorld());
		TaskTimeline->Play();
	}
}

void UPEAim_Task::RevertCameraPosition()
{
	if (!TaskTimeline.IsValid())
	{
		OnFailed.Broadcast();
		EndTask();

		return;
	}
	
	TaskTimeline->Reverse();
}

void UPEAim_Task::OnDestroy(const bool AbilityIsEnding)
{
	UE_LOG(LogGameplayTasks, Display, TEXT("Task %s ended"), *GetName());

	if (TaskTimeline.IsValid() && TaskTimeline->IsPlaying())
	{
		UE_LOG(LogGameplayTasks, Warning, TEXT("Task %s ended while the timeline is playing!"), *GetName());
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UPEAim_Task::TimelineProgress(const float InValue)
{
	CurrentValue = InValue;
	
	if (!TargetCamera.IsValid())
	{
		OnFailed.Broadcast();
		
		TaskTimeline->Stop();
		EndTask();
		
		return;
	}

	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString(__func__) + " - Aiming Position Alpha: " + FString::SanitizeFloat(InValue));
	
	TargetCamera->SetRelativeLocation(FMath::Lerp(CameraInitialPosition, CameraTargetPosition, InValue));
}

void UPEAim_Task::TimelineFinished()
{
	if (CurrentValue == 0.f)
	{
		OnReversionCompleted.Broadcast();
	}
	else if (CurrentValue == CameraLerpTime)
	{
		OnCompleted.Broadcast();		
	}	
}
