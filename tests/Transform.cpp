#include <gtest/gtest.h>

#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"

using namespace Minecraft;

TEST(TransformTests, IdentityTransform) {
  Transform identity = { };
  Transform identityCopy = identity;

  EXPECT_EQ(identity.GetForwardVector(), vec3(0.0, 0.0, -1.0));
  EXPECT_EQ(identity.GetBackwardVector(), vec3(0.0, 0.0, 1.0));
  EXPECT_EQ(identity.GetLeftVector(), vec3(-1.0, 0.0, 0.0));
  EXPECT_EQ(identity.GetRightVector(), vec3(1.0, 0.0, 0.0));
  EXPECT_EQ(identity.GetUpVector(), vec3(0.0, 1.0, 0.0));
  EXPECT_EQ(identity.GetDownVector(), vec3(0.0, -1.0, 0.0));

  EXPECT_EQ(identity.GetTransformationMatrix(), glm::identity<mat4>());

  // Ensure no mutation
  EXPECT_EQ(identity.Position, identityCopy.Position);
  EXPECT_EQ(identity.Rotation, identityCopy.Rotation);
  EXPECT_EQ(identity.Scale, identityCopy.Scale);
}
