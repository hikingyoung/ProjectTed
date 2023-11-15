// Fill out your copyright notice in the Description page of Project Settings.

//将该public目录写入搜索，以后则可以不用再写前面一串
#include "ProjectTed/Public/Ted25DSideScrollCharMovementComp.h"

FRotator UTed25DSideScrollCharMovementComp::ComputeOrientToMovementRotation(const FRotator& CurrentRotation,
	float DeltaTime, FRotator& DeltaRotation) const
{
	//在实际场景中，人物的朝向和Y轴正方向重合。也就是说Y轴是左右，X是向内向外，Z是上下。
	//这里将加速的X方向归零，向内向外移动不再能影响到朝向，会一直面向左或右。但不影响实际加速。
	const FVector Ted25DSideScrollAcceleration = FVector(0,Acceleration.Y,0);
	if (Ted25DSideScrollAcceleration.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		// AI path following request can orient us in that direction (it's effectively an acceleration)
		if (bHasRequestedVelocity && RequestedVelocity.SizeSquared() > KINDA_SMALL_NUMBER)
		{
			const FVector TedRequestedVelocity = FVector(0, RequestedVelocity.Y, 0);
			return TedRequestedVelocity.GetSafeNormal().Rotation();
		}

		// Don't change rotation if there is no acceleration.
		return CurrentRotation;
	}

	// Rotate toward direction of acceleration.
	return Ted25DSideScrollAcceleration.GetSafeNormal().Rotation();
}
