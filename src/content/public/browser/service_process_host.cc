// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/browser/service_process_host.h"

#include "base/strings/utf_string_conversions.h"
#include "content/public/common/content_client.h"

namespace content {

ServiceProcessHost::Options::Options() = default;

ServiceProcessHost::Options::~Options() = default;

ServiceProcessHost::Options::Options(Options&&) = default;

ServiceProcessHost::Options& ServiceProcessHost::Options::WithDisplayName(
    const std::string& name) {
  display_name = base::UTF8ToUTF16(name);
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithDisplayName(
    const std::u16string& name) {
  display_name = name;
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithDisplayName(
    int resource_id) {
  display_name = GetContentClient()->GetLocalizedString(resource_id);
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithChildFlags(
    int flags) {
  child_flags = flags;
  return *this;
}

ServiceProcessHost::Options&
ServiceProcessHost::Options::WithExtraCommandLineSwitches(
    std::vector<std::string> switches) {
  extra_switches = std::move(switches);
  return *this;
}

#if BUILDFLAG(IS_WIN)
ServiceProcessHost::Options& ServiceProcessHost::Options::WithStdoutHandle(
    base::win::ScopedHandle handle) {
  stdout_handle = std::move(handle);
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithStderrHandle(
    base::win::ScopedHandle handle) {
  stderr_handle = std::move(handle);
  return *this;
}
#elif BUILDFLAG(IS_POSIX)
ServiceProcessHost::Options& ServiceProcessHost::Options::WithAdditionalFds(
    base::FileHandleMappingVector mapping) {
  fds_to_remap = std::move(mapping);
  return *this;
}
#endif

ServiceProcessHost::Options& ServiceProcessHost::Options::WithProcessCallback(
    base::OnceCallback<void(const base::Process&)> callback) {
  process_callback = std::move(callback);
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithCurrentDirectory(
    const base::FilePath& cwd) {
  current_directory = cwd;
  return *this;
}

ServiceProcessHost::Options& ServiceProcessHost::Options::WithEnvironment(
    const base::EnvironmentMap& env, bool new_environment) {
  environment = env;
  clear_environment = new_environment;
  return *this;
}

ServiceProcessHost::Options ServiceProcessHost::Options::Pass() {
  return std::move(*this);
}

}  // namespace content
