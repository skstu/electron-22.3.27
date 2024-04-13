// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_UTILITY_SANDBOX_DELEGATE_H_
#define CONTENT_BROWSER_UTILITY_SANDBOX_DELEGATE_H_

#include "base/command_line.h"
#include "base/environment.h"
#include "build/build_config.h"
#include "content/public/common/sandboxed_process_launcher_delegate.h"
#include "content/public/common/zygote/zygote_buildflags.h"
#include "sandbox/policy/mojom/sandbox.mojom.h"

#if BUILDFLAG(USE_ZYGOTE_HANDLE)
#include "content/common/zygote/zygote_handle_impl_linux.h"
#endif  // BUILDFLAG(USE_ZYGOTE_HANDLE)

#if BUILDFLAG(IS_WIN)
#include "sandbox/win/src/sandbox_policy.h"
#endif  // BUILDFLAG(IS_WIN)

namespace content {
class UtilitySandboxedProcessLauncherDelegate
    : public SandboxedProcessLauncherDelegate {
 public:
  UtilitySandboxedProcessLauncherDelegate(sandbox::mojom::Sandbox sandbox_type,
                                          const base::EnvironmentMap& env,
                                          const base::FilePath& cwd,
                                          const base::CommandLine& cmd_line,
                                          bool inherit_environment);
  ~UtilitySandboxedProcessLauncherDelegate() override;

  sandbox::mojom::Sandbox GetSandboxType() override;

#if BUILDFLAG(IS_WIN)
  std::string GetSandboxTag() override;
  bool GetAppContainerId(std::string* appcontainer_id) override;
  bool DisableDefaultPolicy() override;
  bool ShouldLaunchElevated() override;
  bool PreSpawnTarget(sandbox::TargetPolicy* policy) override;
  bool ShouldUnsandboxedRunInJob() override;
  bool CetCompatible() override;
#endif  // BUILDFLAG(IS_WIN)

#if BUILDFLAG(USE_ZYGOTE_HANDLE)
  ZygoteHandle GetZygote() override;
#endif  // BUILDFLAG(USE_ZYGOTE_HANDLE)

  base::EnvironmentMap GetEnvironment() override;
  bool ShouldInheritEnvironment() override;
  base::FilePath GetCurrentDirectory() override;

 private:
  base::EnvironmentMap env_;
  base::FilePath current_directory_;
  sandbox::mojom::Sandbox sandbox_type_;
  base::CommandLine cmd_line_;
  bool inherit_environment_;
};
}  // namespace content

#endif  // CONTENT_BROWSER_UTILITY_SANDBOX_DELEGATE_H_
